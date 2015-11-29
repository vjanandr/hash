#include <string.h>
#include "hashOpen.h"

hashMapOpen::hashMapOpen (int tableLength, hashEnum hashtype) 
    : hashMap(tableLength)
{
    this->hashType = hashtype;
}

hashMapOpen::hashMapOpen (int tableLength)
    : hashMap(tableLength)
{
    this->hashType = HASHING_CRC;
}

hashMapOpen::hashMapOpen ()
{
    this->hashType = HASHING_CRC;
}

uint32_t 
hashMapOpen::hashSimpleModInteger (uint32_t key)
{
    return (key % this->tableLength);
}

uint32_t
hashMapOpen::hashSimpleModString (char *str)
{
    uint32_t len, i=0;
    uint32_t sum = 0;

    if (!str) {
        return 0;
    }

    if (!(len = strlen(str))) {
        return 0;
    }

    while (i <= len && str[i]) {
        sum += str[i];
        i++;
    }
    return (sum % this->tableLength);
}
