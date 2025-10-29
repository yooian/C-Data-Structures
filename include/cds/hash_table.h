#ifndef CDS_HASH_TABLE.H
#define CDS_HASH_TABLE .H

#include <stddef.h>

typedef struct HashTable HashTable;

typedef size_t (*HashFunction)(const void *key);
typedef int (*CompareFunction)(const void *key1, const void *key2);

/** Creates a new empty hash table */
HashTable *ht_create(size_t capacity, HashFunction hash_fn, CompareFunction comp_fn);

/** Destroys and frees memory from a hash table */
void ht_destroy(HashTable *table);

/**
 * Inserts a key-value pair.
 * If the key already exists, the value is updated.
 * Returns 0 on success, non-zero on failure.
 */
int ht_insert(HashTable *table, void *key, void *value);

/**
 * Searches for a key.
 * Returns the associated value if found, or NULL if not found.
 */
void *ht_search(HashTable *table, const void *key);

/**
 * Deletes a key-value pair.
 * Returns 0 on success, non-zero if the key was not found.
 */
int ht_delete(HashTable *table, const void *key);

#endif