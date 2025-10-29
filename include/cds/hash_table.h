#ifndef CDS_HASH_TABLE.H
#define CDS_HASH_TABLE .H

#include <stddef.h>

typedef struct HashTable HashTable;

typedef size_t (*HashFunction)(const void *key);
typedef int (*CompareFunction)(const void *key1, const void *key2);

#endif