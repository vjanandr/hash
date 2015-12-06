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
    
hashMapOpen::hashMapOpen (int tableLength, logger *log)
    : hashMap(tableLength, log)
{
    tableInit();
}

hashMapOpen::hashMapOpen (logger *log)
    : hashMap(log)
{
    tableInit();
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

hashMapOpen::~hashMapOpen()
{
    hashNode *node, *prevNode;
    uint32_t tableIndex = 0;

    while (tableIndex < tableLength) {
        node = hashTable[tableIndex];
        while (node) {
            if (fcbk) {
                fcbk(&node->nodeKey, node->data); 
            }
            prevNode = node;
            node = node->next;
            free(prevNode);
        }
        hashTable[tableIndex] = NULL;
        tableIndex++;
    }
    free(hashTable);
}
