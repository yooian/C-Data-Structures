#include "cds/hash_table.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

int main()
{
    // String test
    HashTable *table = ht_create(10, ht_hash_string, ht_compare_string);
    assert(table != NULL);

    ht_insert(table, "name", "Sky");

    void *name = ht_search(table, "name");
    assert(name != NULL);
    assert(strcmp(name, "Sky") == 0);

    printf("Tests passed!\n");
    return 0;
}