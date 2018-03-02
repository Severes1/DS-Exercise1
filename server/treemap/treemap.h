#define MAX_STRING_SIZE 256

typedef struct TreeMapHeader *TreeMap;

/* Updates an existing entry, or creates a new one if it doesn't exist */
TreeMap treemap_create(int data_size);

/* Returns 1 if the treemap contains the given key, and 0 otherwise. */
int treemap_contains(TreeMap treemap, int key);

/* Updates an existing entry, or creates a new one if it doesn't exist */
int treemap_set(TreeMap treemap, int key, const char* data);

/* Writes the given entry into the given pointers */
int treemap_get(TreeMap treemap, int key, char* data);

/* Removes the given entry from the treemap */
int treemap_delete(TreeMap treemap, int key);

/* Returns the number of entries in the treemap */
int treemap_count(TreeMap treemap);

/* Delete all elements of the treemap and free them */
void treemap_free(TreeMap treemap);
