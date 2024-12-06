/**
 * @file         test_bst.c
 * @brief       Test program for Temperature/Humidity Binary Search Tree ADT
 *
 * This program tests the Binary Search Tree ADT implementation for storing
 * temperature and humidity readings. It verifies basic functionality and error
 * handling by creating a test tree, performing various operations, and checking
 * the results.
 *
 * @author      Danny V. Restrepo (restrepo@pdx.edu)
 * @date        06-Dec-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "temp_humid_bst.h"
#include "iom361_r2.h"



/**************************** Function Prototypes *****************************/

static void greeting(void);
static void test_error_conditions(void);
static void build_test_tree(Tree_t* tree);
static void search_test_cases(Tree_t* tree);
static time_t create_timestamp(int month, int day, int year);



/************************************ Main ************************************/

int main(void) {
    // Displays program introduction and current working directory
    greeting();

    // Initializes iom361 module
    int rtn_code;
    uint32_t* io_base = iom361_initialize(16, 16, &rtn_code);

    if (rtn_code != 0) {
        printf("FATAL(main): Could not initialize I/O module\n");
        return 1;
    }

    printf("INFO(main()): Initialize iom361\n");

    // Tests all error conditions first
    test_error_conditions();

    // Creates main test tree
    Tree_t* tree = create_tree();

    if (tree == NULL) {
        printf("ERROR: Failed to create BST\n");
        return 1;
    }

    // Builds and tests the BST
    build_test_tree(tree);
    
    // Performs search tests
    printf("\nSearching BST for all timestamps... plus a few others\n");
    search_test_cases(tree);

    // Displays final sorted data
    printf("\nTemperature/Humidity table:\n");
    printf("---------------------------\n");
    in_order(tree);

    printf("\n\nTemp & Humid BST ADT test program complete!\n\n");

    return 0;
}



/****************************** Helper Functions ******************************/

/**
 * greeting() - Displays user greeting to console and prints working directory
 */
static void greeting(void) {
   // Displays a greeting and the current working directory
   printf("\n\nBinary Search Tree ADT Test App created by "
          "Danny Restrepo (restrepo@pdx.edu)\n\n");
   
   errno = 0;
   
   // Allocates a buffer large enough to hold the path
   char* buf = getcwd(NULL, 0);
   
   if (buf == NULL) {
      perror("getcwd");
      printf("Could not display the path\n");
   }
   else {
      printf("Current working directory: %s\n", buf);
      free(buf);
   }
   printf("\n");
}



/**
 * create_timestamp() - Creates Unix timestamp for given date
 *
 * @param month   Represents the month (1 - 12)
 * @param day     Represents the day (1 - 31)
 * @param year    Represents the year
 * @return        Unix timestamp for specified date
 */
static time_t create_timestamp(int month, int day, int year) {
    struct tm timeinfo = {0};

    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;

    // Always setting the time to 3pm for consistency between dates
    timeinfo.tm_hour = 15;

    return mktime(&timeinfo);
}



/**
 * test_error_conditions() - Tests BST error handling
 *
 * Performs tests of my error conditions, which include:
 * -> Operations on NULL tree pointers
 * -> Operations with invalid inputs
 * -> Edge cases in tree operations
 */
static void test_error_conditions(void) {
    printf("\n\nTesting error conditions:\n");
    
    // Test 1, operations on a NULL tree
    Data_t test_data = {0};

    printf("\nTest 1: NULL tree operations\n");
    Node_t* result = insert(NULL, test_data);

    if (result != NULL) {
        printf("ERROR: Insert to NULL tree should return NULL\n");
    }
    
    result = search(NULL, time(NULL));

    if (result != NULL) {
        printf("ERROR: Search on NULL tree should return NULL\n");
    }
    
    in_order(NULL);
    
    // Test 2, invalid timestamp search
    printf("\nTest 2: Invalid timestamp search\n");
    Tree_t* test_tree = create_tree();

    if (test_tree != NULL) {
        result = search(test_tree, (time_t) - 1);

        if (result != NULL) {
            printf("ERROR: Search with invalid timestamp should return NULL\n");
        }
    }
    
    printf("\nTest of error conditions complete!\n\n");
}



/**
 * build_test_tree() - Builds test tree with March 2024 data
 *
 * Creates and inserts nodes containing temperature and humidity readings for
 * March 1st to 12th, 2024.
 *
 * @param tree   Pointer to BST to populate
 */
static void build_test_tree(Tree_t* tree) {
    // Create sample data for March 1 - 12, 2024
    // Using the exact temperature/humidity values from sample output
    Data_t readings[12] = {
        {create_timestamp(3,  4, 2024), 0x007AF2E, 0x00D8E24},
        {create_timestamp(3,  8, 2024), 0x007EB95, 0x00D9669},
        {create_timestamp(3, 11, 2024), 0x007F411, 0x00D8EDA},
        {create_timestamp(3, 12, 2024), 0x007D6E8, 0x00C6A4B},
        {create_timestamp(3,  5, 2024), 0x0077D17, 0x00BCD1C},
        {create_timestamp(3,  9, 2024), 0x007DE23, 0x00BE008},
        {create_timestamp(3,  7, 2024), 0x0078A30, 0x00CDB00},
        {create_timestamp(3,  2, 2024), 0x0082489, 0x00C6763},
        {create_timestamp(3,  6, 2024), 0x007F5FB, 0x00CA8B0},
        {create_timestamp(3, 10, 2024), 0x007A124, 0x00CDA24},
        {create_timestamp(3,  3, 2024), 0x0079496, 0x00DB372},
        {create_timestamp(3,  1, 2024), 0x007F62C, 0x00CFE43},
    };
    
    for (int i = 0; i < 12; i++) {
        Node_t* result = insert(tree, readings[i]);

        if (result != NULL) {
            printf("INFO(main()): added data[%d] to BST\n", i);
        }
    }
}



/**
 * search_test_cases() - Tests BST search functionality
 *
 * Performs searches on the BST to verify correct behavior, the following tests
 * are included:
 * -> Searches for existing timestamps
 * -> Searches for non-existent timestamps
 * -> Verifies correct path traversal during searches
 *
 * @param tree   Pointer to BST to search
 */
static void search_test_cases(Tree_t* tree) {
    time_t test_times[] = {
        // Generates timestamps for dates I know exist
        create_timestamp(3, 1, 2024),
        create_timestamp(3, 2, 2024),
        create_timestamp(3, 3, 2024),
        create_timestamp(3, 4, 2024),
        create_timestamp(3, 5, 2024),
        create_timestamp(3, 6, 2024),
        create_timestamp(3, 7, 2024),
        create_timestamp(3, 8, 2024),
        create_timestamp(3, 9, 2024),
        create_timestamp(3, 10, 2024),
        create_timestamp(3, 11, 2024),
        create_timestamp(3, 12, 2024),

        // Generates timestamps for non-existent dates
        create_timestamp(3, 13, 2024),
        create_timestamp(3, 14, 2024)
    };

    // Tests each timestamp
    for (int i = 0; i < sizeof(test_times)/sizeof(time_t); i++) {
        Node_t* result = search(tree, test_times[i]);
        
        char buffer[64];

        struct tm* timeinfo = localtime(&test_times[i]);

        strftime(buffer, sizeof(buffer), "%d-%b-%Y", timeinfo);
        
        if (result != NULL) {
            printf("\nFound data for timestamp %s\n", buffer);
        }
        else {
            printf("Did not find data for timestamp %s\n", buffer);
        }
    }
}