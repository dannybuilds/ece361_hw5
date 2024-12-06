/**
 * @file        temp_humid_bst.h
 * @brief
 * Defines a Binary Search Tree (BST) ADT specialized for storing temperature 
 * and humidity sensor readings with timestamps. The BST maintains data
 * in timestamp-sorted order, allowing efficient searching and ordered traversal.
 * The ADT includes core operations like creation, insertion, search, and 
 * inorder traversal with built-in display functionality.
 *
 * @version     1.0.0
 * @author      Danny V. Restrepo (restrepo@pdx.edu)
 * @date        06-Dec-2024
 */
 
 #ifndef TEMP_HUMID_BST_H
 #define TEMP_HUMID_BST_H
 
 #include <time.h>
 #include <stdint.h>
 #include <stdbool.h>
 

 /*********************** Definitions, Typedefs, Structs ************************/

// Define data item struct to hold sensor reading data
typedef struct temperature_humidity_data {
    time_t timestamp;   // Time the reading was taken
    uint32_t temp;      // Temperature reading from sensor
    uint32_t humid;     // Humidity reading from sensor
} Data_t;



// Define the binary search tree node structure
typedef struct binary_search_tree_node {
    Data_t data;                             // Node's data
    struct binary_search_tree_node* left;    // Pointer to left child
    struct binary_search_tree_node* right;   // Pointer to right child
} Node_t;



// Define the temp/humidity binary search tree structure
typedef struct temperature_humidity_binary_search_tree {
    Node_t* root;       // Pointer to root node of tree
    int node_count;     // Number of nodes in tree
} Tree_t;



/************************** API Function Prototypes ***************************/

/**
 * create_tree() - creates a Temperature/Humidity tree
 *
 * @return	a pointer to the new Temp/Humidity tree if succeeds.  Null if it 
 * fails root node will start as NULL because the tree starts with 0 nodes
 */
 Tree_t* create_tree(void);
 


/**
 * insert() - inserts a temp/humid data record into the tree
 *
 * @param	tree	pointer to the TempHumidtree to add the node to
 * @param	info	Temp/Humid data node to add to tree
 * @return			pointer to the new BST node
 *
 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
Node_t* insert(Tree_t* tree, Data_t info);



/**
 * search() - searches for a temp/humid data record into the tree w/ the 
 *			  specified timestamp
 *
 * @param	tree		pointer to the TempHumidtree to search
 * @param	timestamp	timestamp of the Temp/Humid data node we are seeking
 * @return				pointer to the BST node with that timestamp or NULL if 
 *						not found
 *
 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
 Node_t* search(Tree_t* tree, time_t timestamp);
 
 

/**
 * in_order() - performs in order traversal of tree
 *
 * @param	tree		pointer to the TempHumidtree to add the node to
 *
 * @brief
 * Performs an in order traversal of the BST.  The data in the nodes are
 * displayed using the helper (non-API) function _displayDataNode(). This 
 * function is a wrapper for _inorder() (non-AIPI) which does the work.
 */ 
void in_order(Tree_t* tree);



/**
 * in_order_recursive() - Helper function for recursive in-order traversal
 *
 * @param   node    Current node in traversal
 *
 * @brief
 * Recursively traverses the BST inorder, displaying each node's data, resulting
 * in an ordered list. The order is left subtree, current node, and then the
 * right subtree.
 */
static void in_order_recursive(Node_t* node);



#endif