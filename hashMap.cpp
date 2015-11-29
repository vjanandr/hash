#include "hashMap.h"

#define HASHMAP_DEF_TABLE_LENGTH 100

hashMap::hashMap()
{
    this->tableLength = HASHMAP_DEF_TABLE_LENGTH;
    this->numberOfElements = 0;
}

hashMap::hashMap (int tableLength)
{
    this->tableLength = tableLength;
    this->numberOfElements = 0;
}

int hashMap::getTableLength()
{
    return (this->tableLength);
}

int hashMap::getNumberOfElements()
{
    return (this->numberOfElements);
}
