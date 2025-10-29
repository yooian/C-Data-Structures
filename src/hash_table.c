#include "cds/hash_table.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LOAD_FACTOR 0.7

/* ENUMS */
typedef enum
{
    SLOT_EMPTY,
    SLOT_OCCUPIED,
    SLOT_DELETED // tombstone
} SlotState;

/* STRUCTS */
typedef struct
{
    void *key;
    void *value;
    SlotState state;
} HashTableEntry;

struct HashTable
{
    size_t capacity; // Total num of slots
    size_t size;     // Num of occupied slots

    HashFunction hash_fn;
    CompareFunction comp_fn;

    HashTableEntry *entries; // Array of slots
};

/* PRIVATE FUNCTIONS */
// Idea: just a workhorse func that searches for key in entry list. Returns pointer to a slot
static HashTableEntry *ht_find_slot(HashTable *table, const void *key, HashTableEntry **p_first_tombstone)
{
    // go through entries
    // if we're searching, we just look for matching key and return either slot (if found) or NULL/empty
    // if we're deleting, same thing. In the delete func, we set the status to deleted
    // if inserting, same thing but we store any passing tombstones. In insert func, we insert into either first tombstone or empty
}

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

/* Convenience Functions */
size_t ht_hash_string(const void *key);
int ht_compare_string(const void *key1, const void *key2);

size_t ht_hash_int(const void *key);
int ht_compare_int(const void *key1, const void *key2);

size_t ht_hash_float(const void *key);
int ht_compare_float(const void *key1, const void *key2);