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
static HashTableEntry *ht_find_slot(HashTable *table, const void *key, HashTableEntry **first_tombstone)
{
    size_t hash = table->hash_fn(key);
    // go through entries
    for (size_t i = 0; i < table->capacity; ++i)
    {
        // i*i is simplest quadratic probe
        size_t index = (hash + i * i) % table->capacity;
        HashTableEntry *entry = &table->entries[index];

        if (entry->state == SLOT_OCCUPIED)
        {
            // if we're searching, we just look for matching key and return either slot (if found) or NULL/empty
            // if we're deleting, same thing. In the delete func, we set the status to deleted
            if (table->comp_fn(entry->key, key) == 0)
            {
                return entry;
            }
        }
        else if (entry->state == SLOT_DELETED)
        {
            // if inserting, same thing but we store any passing tombstones. In insert func, we insert into either first tombstone or empty
            if (first_tombstone != NULL && *first_tombstone == NULL)
                *first_tombstone = entry;
        }
        else if (entry->state == SLOT_EMPTY)
        {
            return entry;
        }
    }
    return NULL;
}

/** Creates a new empty hash table */
HashTable *ht_create(size_t capacity, HashFunction hash_fn, CompareFunction comp_fn)
{
    HashTable *table = malloc(sizeof(HashTable));
    table->capacity = capacity;
    table->hash_fn = hash_fn;
    table->comp_fn = comp_fn;
    table->size = 0;
    table->entries = calloc(capacity, sizeof(HashTableEntry));
    return table;
}

/** Destroys and frees memory from a hash table */
void ht_destroy(HashTable *table)
{
    // expect users to free their own keys/values
    free(table->entries);
    free(table);
}

/**
 * Inserts a key-value pair.
 * If the key already exists, the value is updated.
 * Returns 0 on success, non-zero on failure.
 */
int ht_insert(HashTable *table, void *key, void *value)
{
    HashTableEntry *entry;
    HashTableEntry *first_tombstone = NULL;
    entry = ht_find_slot(table, key, &first_tombstone);
    if (entry->state == SLOT_OCCUPIED)
    {
        entry->value = value;
        return 0;
    }
    else if (entry->state == SLOT_EMPTY || entry->state == SLOT_DELETED)
    {
        HashTableEntry *entry_slot = entry;
        if (first_tombstone != NULL)
        {
            entry_slot = first_tombstone;
        }
        entry_slot->state = SLOT_OCCUPIED;
        entry_slot->key = key;
        entry_slot->value = value;
        table->size++;
        return 0;
    }
    return -1;
}

/**
 * Searches for a key.
 * Returns the associated value if found, or NULL if not found.
 */
void *ht_search(HashTable *table, const void *key)
{
    HashTableEntry *entry;
    entry = ht_find_slot(table, key, NULL);

    if (entry != NULL && entry->state == SLOT_OCCUPIED)
    {
        return entry->value;
    }
    return NULL;
}

/**
 * Deletes a key-value pair.
 * Returns 0 on success, non-zero if the key was not found.
 */
int ht_delete(HashTable *table, const void *key)
{
    HashTableEntry *entry;
    entry = ht_find_slot(table, key, NULL);

    if (entry != NULL && entry->state == SLOT_OCCUPIED)
    {
        entry->state = SLOT_DELETED;
        table->size--;
        return 0;
    }
    return -1;
}

/* Convenience Functions */
size_t ht_hash_string(const void *key)
{
    // djb2 hash alg
    unsigned long hash = 5381;
    int c;
    const unsigned char *str = (const unsigned char *)key;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (size_t)hash;
}
int ht_compare_string(const void *key1, const void *key2)
{
    return strcmp((const char *)key1, (const char *)key2);
}

size_t ht_hash_int(const void *key)
{
    int int_key = *(const int *)key;
    // placeholder hash based on FNV-1a - consider murmur later
    return (size_t)int_key * 2654435761u;
}
int ht_compare_int(const void *key1, const void *key2)
{
    int int1 = *(const int *)key1;
    int int2 = *(const int *)key2;
    return int1 - int2;
}

// floats are annoying for now
// size_t ht_hash_float(const void *key);
// int ht_compare_float(const void *key1, const void *key2);