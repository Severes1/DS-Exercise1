#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"
#define TRUE 1
#define FALSE 0

/* Creates a new TreeMapNode node with the given info */
TreeMapNode treemap_node_create(int key, char* value1, float value2) {
    TreeMapNode new_treemap = malloc(sizeof(TreeMapStruct));
    new_treemap->key = key;
    memcpy(new_treemap->value1, value1, MAX_STRING_SIZE); 
    new_treemap->value2 = value2;
    new_treemap->left = NULL;
    new_treemap->right = NULL;
    return new_treemap;
}

/* Returns 1 if the treemap contains the given key, and 0 otherwise. */
int treemap_node_contains(TreeMapNode treemap, int key) {
    if (treemap == NULL) {
        return FALSE;
    }
    if (treemap->key == key) {
        return TRUE;
    } 
    if (treemap_node_contains(treemap->left, key)) {
        return TRUE;
    }
    if (treemap_node_contains(treemap->right, key)) {
        return TRUE;
    }
    return FALSE;
}

/* Updates an existing entry, or creates a new one if it doesn't exist */
int treemap_node_set(TreeMapNode treemap, int key, char* value1, float value2) {
    if (treemap == NULL) {
        return -1;
    }
    if (treemap->key == key) {
        memcpy(treemap->value1, value1, MAX_STRING_SIZE);
        treemap->value2 = value2; 
        return 0;
    } else if (key < treemap->key) {
        if (treemap->left == NULL) {
            treemap->left = treemap_node_create(key, value1, value2);
            return 0;
        } else {
            return treemap_node_set(treemap->left, key, value1, value2); 
        }
    } else {
        if (treemap->right == NULL) {
            treemap->right = treemap_node_create(key, value1, value2);
            return 0;
        } else {
            return treemap_node_set(treemap->right, key, value1, value2);
        }
    }
}

/* Writes the given entry into the given pointers */
int treemap_node_get(TreeMapNode treemap, int key, char* value1, float *value2) {
    if (treemap == NULL) {
        return -1;
    }
    if (key == treemap->key) {
        memcpy(value1, treemap->value1, MAX_STRING_SIZE);
        *value2 = treemap->value2;
        return 0;
    } else if (key < treemap->key) {
        return treemap_node_get(treemap->left, key, value1, value2);
    } else {
        return treemap_node_get(treemap->right, key, value1, value2);
    }
}

/* Affixes the left_node to the leftmost leaf of the right_node */
TreeMapNode patch(TreeMapNode left_node, TreeMapNode right_node) {
    if (right_node == NULL) {
        return left_node;
    }
    if (right_node->left == NULL) {
        right_node->left = left_node;
        return right_node;
    } else {
        return patch(left_node, right_node->left);
    }
}

/* Removes the given entry from the treemap. 0 success, -1 error */
int treemap_node_delete(TreeMapNode treemap, int key) {
    if (treemap == NULL) {
        return -1;
    }
    if (key == treemap->key) {
        return -2; // cannot remove the root entry. Make a new TreeMapNode instead.
    }
    if (key < treemap->key) {
        if (treemap->left == NULL) {
            return -1; // Entry does not exist
        } else if (treemap->left->key == key) {
            TreeMapNode temp = patch(treemap->left->left, treemap->left->right);
            free(treemap->left);
            treemap->left = temp;
            return 0;
        } else {
            return treemap_node_delete(treemap->left, key);
        }
    } else {
        if (treemap->right == NULL) {
            return -1; // Entry does not exist
        } else if (treemap->right->key == key) {
            TreeMapNode temp = patch(treemap->right->left, treemap->right->right);
            free(treemap->right);
            treemap->right = temp;
            return 0;
        } else {
            return treemap_node_delete(treemap->right, key);
        }
    }
}

/* Counts the number of entries in a treemap */
int treemap_node_count(TreeMapNode treemap) {
    if (treemap == NULL) {
        return 0;
    } else {
        return treemap_node_count(treemap->left) 
            + treemap_node_count(treemap->right) + 1;
    }
}

// Creates a new treemap
TreeMap treemap_create() {
    TreeMap header = malloc(sizeof(struct TreeMapHeader));
    header->contents = NULL;
    return header;
}

/* Returns 1 if the treemap contains the given key, and 0 otherwise. */
int treemap_contains(TreeMap treemap, int key) {
    if (treemap->contents == NULL) {
        return FALSE;
    } else {
        return treemap_node_contains(treemap->contents, key);
    }
}

/* Updates an existing entry, or creates a new one if it doesn't exist */
int treemap_set(TreeMap treemap, int key, char* value1, float value2) {
    if (treemap->contents == NULL) {
        treemap->contents = treemap_node_create(key, value1, value2);
    }
    return treemap_node_set(treemap->contents, key, value1, value2);
}

/* Writes the given entry into the given pointers */
int treemap_get(TreeMap treemap, int key, char* value1, float *value2) {
    return treemap_node_get(treemap->contents, key, value1, value2);
}

/* Removes the given entry from the treemap */
int treemap_delete(TreeMap treemap, int key) {
    int retval = treemap_node_delete(treemap->contents, key);
    if (retval == -2) {
        if (treemap->contents == NULL) {
            return -1;
        }
        TreeMapNode temp = patch(treemap->contents->left, treemap->contents->right);
        free(treemap->contents);
        treemap->contents = temp;
        return 0;
    } else {
        return retval;
    }
}

/* Returns the number of entries in the treemap */
int treemap_count(TreeMap treemap) {
    return treemap_node_count(treemap->contents);
}

void treemap_node_free(TreeMapNode node) {
    if (node == NULL) {
        return;
    } 
    if (node->left != NULL) {
        treemap_node_free(node->left);
    }
    if (node->right != NULL) {
        treemap_node_free(node->right);
    }
    free(node);
}

/* Deletes all elements of the tree */
void treemap_free(TreeMap treemap) {
    treemap_node_free(treemap->contents);
    free(treemap);
}
