/**
 * @file        temp_humid_bst.c
 * @brief
 * Implements the Binary Search Tree ADT functions defined in temp_humid_bst.h.
 * Provides creation, insertion, search, and traversal operations for handling
 * temperature and humidity readings with timestamps.
 *
 * @version     1.0.0
 * @author      Danny V. Restrepo (restrepo@pdx.edu)
 * @date        06-Dec-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "temp_humid_bst.h"



/************************ API Function Implementations ************************/

Tree_t* create_tree(void) {
    // Allocate memory for the tree structure
    Tree_t* new_tree = (Tree_t*)malloc(sizeof(Tree_t));

    // Initialize tree if allocation succeeded
    if (new_tree != NULL) {
        new_tree->root = NULL;
        new_tree->node_count = 0;
        printf("INFO(create_tree()): Successfully created a "
               "Temp/Humidity tree.\n");
    }
    else {
        printf("ERROR(create_tree()): Failed to create tree.\n");
    }

    return new_tree;
}



Node_t* insert(Tree_t* tree, Data_t info) {
    // Validate input parameters
    if (tree == NULL) {
        printf("ERROR(insert()): Cannot insert into NULL tree.\n");
        return NULL;
    }

    // Create and initialize new node
    Node_t* new_node = (Node_t*)malloc(sizeof(Node_t));
    if (new_node == NULL) {
        printf("ERROR(insert()): Failed to allocate memory for new node.\n");
        return NULL;
    }
    
    // Set up the new node's data and pointers
    new_node->data = info;
    new_node->left = NULL;
    new_node->right = NULL;

    // Handle empty tree case
    if (tree->root == NULL) {
        tree->root = new_node;
        tree->node_count++;
        printf("INFO(insert()): Tree is empty... inserting root node.\n");
        return new_node;
    }

    // Find insertion point in non-empty tree
    printf("INFO(insert()): Tree is populated... "
           "looking for insertion point.\n");
    Node_t* current = tree->root;
    Node_t* parent = NULL;
    
    while (current != NULL) {
        parent = current;
        
        // Navigate based on timestamp comparison
        if (info.timestamp < current->data.timestamp) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    
    // Insert node at found position
    if (info.timestamp < parent->data.timestamp) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }
    
    tree->node_count++;

    return new_node;
}



Node_t* search(Tree_t* tree, time_t timestamp) {
    if (tree == NULL) {
        printf("ERROR(search()): Cannot search NULL tree.\n");
        return NULL;
    }

    // Validate timestamp
    if (timestamp < 0) {
        printf("ERROR(search()): Invalid timestamp %ld.\n", timestamp);
        return NULL;
    }

    // Continue with normal search operation
    printf("INFO(search()): Starting search for timestamp %ld.\n", timestamp);
    printf("INFO(search()): Visiting these nodes:\n");
    
    Node_t* current = tree->root;
    
    // Search until we find the timestamp or hit a leaf
    while (current != NULL && current->data.timestamp != timestamp) {
        char date_str[26];
        strftime(date_str,
                 sizeof(date_str), 
                 "%c", 
                 localtime(&current->data.timestamp));
        printf("-> [%ld] %s\n", current->data.timestamp, date_str);
        
        if (timestamp < current->data.timestamp) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    
    // Report if timestamp was found
    if (current != NULL) {
        char date_str[26];
        strftime(date_str,
                 sizeof(date_str), 
                 "%c", 
                 localtime(&current->data.timestamp));
        printf("FOUND -> %s\n", date_str);
    }
    
    return current;
}



void in_order(Tree_t* tree) {
    if (tree == NULL) {
        printf("ERROR(in_order()): Cannot traverse NULL tree.\n");
        return;
    }

    printf("INFO(in_order()): There are %d nodes in the BST.\n", 
           tree->node_count);
    
    // Start recursive traversal from root
    in_order_recursive(tree->root);
}



static void in_order_recursive(Node_t* node) {
    if (node != NULL) {
        // Traverse left subtree
        in_order_recursive(node->left);
        
        // Display current node's data
        char date_str[26];

        strftime(date_str, sizeof(date_str), "%d-%b-%Y", 
                 localtime(&node->data.timestamp));

        printf("%s     %08X %08X\n", 
               date_str, node->data.temp, node->data.humid);
        
        // Traverse right subtree
        in_order_recursive(node->right);
    }
}