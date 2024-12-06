/**
 * @file        hw5_app.c
 * @brief
 * This program populates a binary search tree with temperature and humidity
 * data given a start date and time span designated by the user, allows the user
 * to search for specific data entries, and displays an ordered table of all the
 * readings. Uses the iom361_r2 IO module and C time library.
 *
 * @version     1.0.0
 * @author      Danny V. Restrepo (restrepo@pdx.edu)
 * @date        06-Dec-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "temp_humid_bst.h"
#include "iom361_r2.h"



/**************************** Function Prototypes *****************************/

static void greeting(void);
static void shuffle(Data_t* array, size_t n);
void populateBST(Tree_t* tree, int month, int day, int num_days);



/************************************ Main ************************************/

int main() {
    int start_month, start_day, num_days;
    
    // Displays program introduction and current working directory
    greeting();

    // Creates and initializes the BST
    Tree_t* tree = create_tree();
    if (tree == NULL) {
        printf("ERROR(main()): Failed to create tree\n");
        return 1;
    }

    // Get input parameters from user
    printf("Enter the starting month (1 to 12),day (1 to 31), "
           "and number of days (1 to 100): ");
    if (scanf("%d,%d,%d", &start_month, &start_day, &num_days) != 3) {
        printf("ERROR(main()): Invalid input format\n");
        delete_tree(tree);
        return 1;
    }

    printf("User requested %d data items starting at %2d/%2d/2023\n",
           num_days, start_month, start_day);

    // Populates the BST with random readings
    populateBST(tree, start_month, start_day, num_days);

    // Processes search requests
    char date_input[20];

    // Gets rid of newline from scanf
    getchar();

    while (1) {
        printf("\nEnter a search date (mm/dd/yyyy): ");
        if (fgets(date_input, sizeof(date_input), stdin) == NULL || 
            date_input[0] == '\n') {
            break;
        }

        // Parses the search date
        struct tm search_time = {0};
        if (strptime(date_input, "%m/%d/%Y", &search_time) == NULL) {
            printf("ERROR(main()): Invalid date format. Use mm/dd/yyyy\n");
            continue;
        }
        
        // Sets time to 1 PM to match data
        search_time.tm_hour = 13;
        time_t search_timestamp = mktime(&search_time);

        // Searches the BST
        Node_t* result = search(tree, search_timestamp);
        
        char date_str[26];
        strftime(date_str, sizeof(date_str), "%d-%b-%Y", 
                 localtime(&search_timestamp));

        if (result == NULL) {
            printf("Did not find data for Timestamp %s\n", date_str);
        }
        else {
            printf("Found data for Timestamp %s\n", date_str);
            printf("%s     %08X (%05.1fF) %08X (%05.1f%%)\n",
                   date_str,
                   result->data.temp,
                   (float)result->data.temp / 1000.0,
                   result->data.humid,
                   (float)result->data.humid / 1000.0);
        }
    }

    // Displays ordered table of readings
    printf("\nTemperature/Humidity table:\n");
    printf("---------------------------\n");

    in_order(tree);

    delete_tree(tree);

    return 0;
}



/****************************** Helper Functions ******************************/

/**
 * greeting() - displays program introduction and working directory
 */
static void greeting(void) {
   // Displays a greeting and the current working directory
   printf("\n\nThe Homework 5 Application created by "
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
 * populateBST() - Populates the binary search tree with randomly generated 
 *                 temperature and humidity data.
 *
 * Uses the C time library mktime(), iom361_setSensor1_rndm() and 
 * iom361_readReg() functions from the iom361_r2 module library provided to us
 * Prof. Kravitz to generate random temperature and humidity readings. Each 
 * reading is assigned a timestamp based on a user-provided starting date 
 * (month and day) and the number of designated days. The data is then inserted
 * into the binary search tree in a randomized order to maintain the optimal
 * performance of the BST as described in the lecture slides.
 *
 * @param tree          Pointer to the binary search tree to be populated
 * @param month         Represents the starting month for the data (1 - 12)
 * @param day           Represents the starting day for the data (1 - 31)
 * @param num_days      Represents number of days of data to generate (1 - 100)
 */
void populateBST(Tree_t* tree, int month, int day, int num_days) {
    // Validates input parameters
    if (tree == NULL || month < 1 || month > 12 || 
        day < 1 || day > 31 || num_days < 1) {
        printf("ERROR(populateBST()): Invalid parameters\n");
        return;
    }

    // Initializes iom361
    int rtn_code;
    uint32_t* base = iom361_initialize(16, 16, &rtn_code);
    
    if (base == NULL || rtn_code != 0) {
        printf("ERROR(populateBST()): Failed to initialize iom361.\n");
        return;
    }

    // Creates array to track original indices
    int* indices = malloc(num_days * sizeof(int));
    Data_t* readings = malloc(num_days * sizeof(Data_t));

    if (!indices || !readings) {
        free(indices);
        free(readings);
        printf("ERROR(populateBST()): Memory allocation failed.\n");
        return;
    }

    // Initializes time structure for starting date
    struct tm start_time = {0};
    start_time.tm_year = 2023 - 1900;
    start_time.tm_mon = month - 1;     
    start_time.tm_mday = day;
    start_time.tm_hour = 13;           

    time_t current_time = mktime(&start_time);
    
    // Generates readings and indices
    for (int i = 0; i < num_days; i++) {
        readings[i].timestamp = current_time;

        // Tracks original position
        indices[i] = i;
        
        _iom361_setSensor1_rndm(50.0, 85.0, 40.0, 85.0);
        readings[i].temp = iom361_readReg(base, TEMP_REG, &rtn_code);
        readings[i].humid = iom361_readReg(base, HUMID_REG, &rtn_code);
        
        current_time += 86400;
    }

    // Shuffles both arrays together
    srand(time(NULL));
    for (int i = num_days - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // Swaps readings
        Data_t temp_reading = readings[i];
        readings[i] = readings[j];
        readings[j] = temp_reading;

        // Swaps corresponding indices
        int temp_index = indices[i];
        indices[i] = indices[j];
        indices[j] = temp_index;
    }

    // Inserts shuffled readings while preserving original indices
    for (int i = 0; i < num_days; i++) {
        printf("INFO(main()): added timestamp %ld from data[%d] to BST\n", 
               readings[i].timestamp, indices[i]);

        insert(tree, readings[i]);
    }

    free(indices);
    free(readings);
}



/**
 * @brief Shuffles an array of Data_t elements randomly
 *
 * Uses the Fisher-Yates shuffle algorithm to randomize array order in order to
 * optimize BST performance, as described in the lecture slides.
 *
 * @param array     Pointer to array of Data_t elements to shuffle
 * @param n         Number of elements in the array
 */
static void shuffle(Data_t* array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);

            Data_t temp = array[j];

            array[j] = array[i];
            array[i] = temp;
        }
    }
}