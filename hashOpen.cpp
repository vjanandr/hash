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
hashMapOpen::getHashKey (hashNodeKey *nodeKey)
{
    uint32_t hkey;

    switch (hashType) {
        case HASHING_SIMPLE_MOD_INTEGER:
            hkey = hashSimpleModInteger(nodeKey->intKey);
            break;
        case HASHING_SIMPLE_MOD_STRING:
            hkey = hashSimpleModString(nodeKey->strKey, nodeKey->keyByteLength);
            break;
        case HASHING_CRC:
            hkey = hashCRC(nodeKey->byteKey, nodeKey->keyByteLength);
            break;
    }
    return hkey;
}

apiRetVal hashMapOpen::add (hashNodeKey *key, void *data);
{
    uint32_t hashKey;
    listNode *node, *newNode = NULL;

    if (data) {
        return (API_RETVAL_INVALID_INPUT);
    }
    hashKey = getHashKey(key);
    node = hashTable[hashKey];
    newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = data;
    newNode->nodeKey = *key;
    hashTable[hashKey] = newNode;
    newNode->next = node;
    newNode->nodeState = HASH_NODE_USED;
    numberOfElements++;
    return API_RETVAL_SUCCESS;
}

apiRetVal 
hashMapOpen::walk (walkcbk cbk)
{
    listNode *node;
    uint32_t tableIndex = 0;

    if (!cbk) {
        return (API_RETVAL_INVALID_INPUT);
    }
    while (tableIndex <= tableLength) {
        node = hashTable[tableIndex];
        while (node) {
            if (cbk(node->data) != CBK_RET_CONTINUE) {
                return API_RETVAL_SUCCESS;
            }
            node = node->next;
        }
        tableIndex++;
    }
    return API_RETVAL_SUCCESS;
}

apiRetVal
hashMapOpen::find (hashNodeKey *key, void **data)
{
    hashNode *node = NULL;

    hashKey = getHashKey(key);
    node = hashTable[hashKey];
    while (node) {
        if (keyCmp(key, node->nodeKey)) {
            *data = node->data;
            return (API_RETVAL_SUCCESS);
        }
        node = node->next;
    }
    return API_RETVAL_DATA_NOT_FOUND;
}

apiRetVal
hashMapOpen::remove (hashNodeKey *key, void **data)
{
    hashNode *node = NULL, *prevNode = NULL;

    hashKey = getHashKey(key);
    node = hashTable[hashKey];
    while (node) {
        if (keyCmp(key, node->nodeKey)) {
            *data = node->data;
            if (!prevNode) {
                // This is the first node.
                hashTable[hashKey] = node->next;
            } else {
                prevNode->next = node->next;
            }
            return (API_RETVAL_SUCCESS);
        }
        prevNode = node;
        node = node->next;
    }
    return API_RETVAL_DATA_NOT_FOUND;
}
