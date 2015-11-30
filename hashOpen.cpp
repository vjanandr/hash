#include <string.h>
#include "hashOpen.h"
#include "listNode.h"

void hashMapOpen::tableInit ()
{
    this->hashTable = (listNode **)malloc(sizeof(listNode *) * 
                                         this->tableLength);
    memset(this->hashTable, sizeof(listNode *) * this->tableLength, 0);
}
    

hashMapOpen::hashMapOpen (int tableLength, hashEnum hashtype) 
    : hashMap(tableLength)
{
    this->hashType = hashtype;
    tableInit();
}

hashMapOpen::hashMapOpen (int tableLength)
    : hashMap(tableLength)
{
    this->hashType = HASHING_CRC;
    tableInit();
}

hashMapOpen::hashMapOpen ()
{
    this->hashType = HASHING_CRC;
    tableInit();
}

uint32_t 
hashMapOpen::hashSimpleModInteger (uint32_t key)
{
    return (key % this->tableLength);
}

uint32_t
hashMapOpen::hashSimpleModString (char *str, uint32_t bytelength)
{
    uint32_t i = 0, sum = 0;

    if (!str) {
        return 0;
    }

    while (i <= bytelength && str[i]) {
        sum += str[i];
        i++;
    }
    return (sum % this->tableLength);
}

uint32_t 
hashMapOpen::getHashKey (void *data, uint32_t byteLength) 
{
    uint32_t hkey;

    switch (hashType) {
        case HASHING_SIMPLE_MOD_INTEGER:
            hkey = hashSimpleModInteger(*((int *)data));
            break;
        case HASHING_SIMPLE_MOD_STRING:
            hkey = hashSimpleModString((char *)data, byteLength);
            break;
        case HASHING_CRC:
            hkey = hashCRC((char *)data, byteLength);
            break;
    }
    return hkey;
}

apiRetVal hashMapOpen::add (void *data, uint32_t byteLength, comparecbk cbk)
{
    uint32_t hashKey;
    listNode *node, *newNode = NULL;

    if (data) {
        return (API_RETVAL_INVALID_INPUT);
    }
    hashKey = getHashKey(data, byteLength);
    node = hashTable[hashKey];
    newNode = (listNode *)malloc(sizeof(listNode));
    hashTable[hashKey] = newNode;
    newNode->next = node;
}
