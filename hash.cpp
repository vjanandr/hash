#include "hash.h"

#define HASHMAP_DEF_TABLE_LENGTH 100

hashMap::hashMap()
{
    this->table_length = HASHMAP_DEF_TABLE_LENGTH;
    this->number_of_elements = 0;
}

hashMap::hashMap (int table_length)
{
    this->table_length = table_length;
    this->number_of_elements = 0;
}

int hashMap::getTableLength()
{
    return (this->table_length);
}

int hashMap::getNumberOfElements()
{
    return (this->number_of_elements);
}
