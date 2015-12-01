#include <string.h>
#include "hashMapOpen.h"
#include "hashNode.h"

void hashMapOpen::tableInit ()
{
    this->hashTable = (hashNode **)malloc(sizeof(hashNode *) * 
                                         this->tableLength);
    memset(this->hashTable, sizeof(hashNode *) * this->tableLength, 0);
    log->debug("Hash Open map Table Length %d\n", tableLength);
}
    

hashMapOpen::hashMapOpen (int tableLength, hashEnum hashtype, logger *log) 
    : hashMap(tableLength, log)
{
    this->hashType = hashtype;
    tableInit();
}

hashMapOpen::hashMapOpen (int tableLength, logger *log)
    : hashMap(tableLength, log)
{
    this->hashType = HASHING_CRC;
    tableInit();
}

hashMapOpen::hashMapOpen (logger *log)
    : hashMap(log)
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
hashMapOpen::hashCRC (uint8_t *bytes,  uint32_t keyByteLength)
{
    return (0);
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
    log->verbose("Hkey generated %d\n", hkey);
    return hkey;
}

apiRetVal hashMapOpen::add (hashNodeKey *key, void *data)
{
    uint32_t hashKey;
    hashNode *node, *newNode = NULL;

    if (!data) {
        log->error("Null data while adding\n");
        return (API_RETVAL_INVALID_INPUT);
    }
    hashKey = getHashKey(key);
    node = hashTable[hashKey];
    newNode = (hashNode *)malloc(sizeof(hashNode));
    newNode->data = data;
    newNode->nodeKey = *key;
    hashTable[hashKey] = newNode;
    newNode->next = node;
    newNode->nodeState = HASH_NODE_USED;
    numberOfElements++;
    log->info("Data added to hash table \n");
    return API_RETVAL_SUCCESS;
}

apiRetVal 
hashMapOpen::walk (walkcbk cbk)
{
    hashNode *node;
    uint32_t tableIndex = 0;

    if (!cbk) {
        log->error("Null cbk in walk\n");
        return (API_RETVAL_INVALID_INPUT);
    }
    while (tableIndex < tableLength) {
        node = hashTable[tableIndex];
        while (node) {
            if (cbk(&node->nodeKey, node->data) != CBK_RET_CONTINUE) {
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
    uint32_t hashKey;
    hashNode *node = NULL;

    hashKey = getHashKey(key);
    if (hashKey > tableLength) {
        return API_RETVAL_INVALID_INPUT;
    }

    node = hashTable[hashKey];
    while (node) {
        if (keyCmp(key, &node->nodeKey)) {
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
    uint32_t hashKey;
    hashNode *node = NULL, *prevNode = NULL;

    hashKey = getHashKey(key);
    if (hashKey > tableLength) {
        return API_RETVAL_INVALID_INPUT;
    }
    node = hashTable[hashKey];

    while (node) {
        if (keyCmp(key, &node->nodeKey)) {
            *data = node->data;
            if (!prevNode) {
                // This is the first node.
                hashTable[hashKey] = node->next;
            } else {
                prevNode->next = node->next;
            }
            free(node);
            return (API_RETVAL_SUCCESS);
        }
        prevNode = node;
        node = node->next;
    }
    return API_RETVAL_DATA_NOT_FOUND;
}
