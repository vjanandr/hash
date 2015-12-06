#include "hashMapClosed.h"
#include <math.h>
#define MAX_ITER 2

const char* 
hashMapClosed::NodeStateString (uint32_t nodeState)
{
    std::string nodeStr = NULL;

    if (nodeState & HASH_NODE_USED) {
        nodeStr += "NODE_USED";
    }
    if (nodeState & HASH_NODE_EMPTY) {
        nodeStr +=":NODE_EMPTY";
    }
    if (nodeState & HASH_NODE_DELETED) {
        nodeStr += ":NODE_DELETED";
    }
    return nodeStr.data();
}

uint32_t
hashMapClosed::getPositionLinearProbing (hashNodeKey *key, uint32_t nodeState)
{
    hashNode *node = NULL;
    uint32_t iter = 1, iterCnt = 0;
    uint32_t position, hkey;

    hkey = getHashKey(key);
    position = hkey;
    while (true) {
        position = position % tableLength;
        node = &hashTable[position];

        if (nodeState & HASH_NODE_USED) {
            if ((node->nodeState & HASH_NODE_USED) &&
                (keyCmp(key, &node->nodeKey))) {
                return position;
            }
            if (node->nodeState & HASH_NODE_EMPTY) {
                return (tableLength + 1);
            }
        } else if (nodeState & (HASH_NODE_EMPTY | HASH_NODE_DELETED)) {
            if (node->nodeState & nodeState) {
                return position;
            }
        } else {
            // Something wrong;
            log->error("Invalid node state %d \n",nodeState);
            return (tableLength + 1);
        }
        if (iterCnt > MAX_ITER * tableLength) {
            log->error("Max Iter reached node not found %s\n",
                        NodeStateString(nodeState));
            return (tableLength + 1);
        }
        iterCnt++;
        position = (hkey + iter++);
    }
}

uint32_t
hashMapClosed::getPositionQuadProbing (hashNodeKey *key, uint32_t nodeState)
{
    hashNode *node = NULL;
    uint32_t itercnt = 0;
    uint32_t iter = 1;
    uint32_t position, hkey;

    hkey = getHashKey(key);
    position = hkey;
    while (true) {
        position = position % tableLength;
        node = &hashTable[position];

        if (nodeState & HASH_NODE_USED) {
            if ((node->nodeState & HASH_NODE_USED) &&
                (keyCmp(key, &node->nodeKey))) {
                return position;
            }
            if (node->nodeState & HASH_NODE_EMPTY) {
                return (tableLength + 1);
            }
        } else if (nodeState & (HASH_NODE_EMPTY | HASH_NODE_DELETED)) {
            if (node->nodeState & nodeState) {
                return position;
            }
        } else {
            // something wrong;
            log->error("Invalid node state %d \n",nodeState);
            return (tableLength + 1);
        }
        if (itercnt > MAX_ITER * tableLength) {
            log->error("max iter reached node not found \n");
            return (tableLength + 1);
        }
        itercnt++;
        position = (hkey + iter * iter);
        iter++;
    }
}

uint32_t 
hashMapClosed::getPositionDoubleProbing (hashNodeKey *key, uint32_t nodeState)
{
    hashNode *node = NULL;
    uint32_t itercnt = 0;
    uint32_t iter = 1;
    uint32_t position, hkey1, hkey2;

    hkey1 = getHashKey(key);
    hkey2 = 7 - getHashKey(key, 7);
    if (!hkey2) {
        hkey2 = 1 + getHashKey(key, 7);
    }
    position = hkey1;
    while (true) {
        position = position % tableLength;
        node = &hashTable[position];

        if (nodeState & HASH_NODE_USED) {
            if ((node->nodeState & HASH_NODE_USED) &&
                (keyCmp(key, &node->nodeKey))) {
                return position;
            }
            if (node->nodeState & HASH_NODE_EMPTY) {
                return (tableLength + 1);
            }
        } else if (nodeState & (HASH_NODE_EMPTY | HASH_NODE_DELETED)) {
            if (node->nodeState & nodeState) {
                return position;
            }
        } else {
            // something wrong;
            log->error("Invalid node state %d \n",nodeState);
            return (tableLength + 1);
        }
        if (itercnt > MAX_ITER * tableLength) {
            log->error("max iter reached node not found \n");
            return (tableLength + 1);
        }
        itercnt++;
        position = (hkey1 + iter * hkey2);
        iter++;
    }
}

uint32_t
hashMapClosed::getPosition (hashNodeKey *key, uint32_t nodeState)
{
    uint32_t position = 0;
    switch (hashType) {
        case HASH_CLOSED_LINEAR:
            position = getPositionLinearProbing(key, nodeState);
            break;
        case HASH_CLOSED_QUADRATIC:
            position = getPositionQuadProbing(key, nodeState);
            break;
        case HASH_CLOSED_DOUBLE:
            position = getPositionDoubleProbing(key, nodeState);
            break;
    }
    return position;
}

void
hashMapClosed::tableInit()
{
    this->hashTable = (hashNode *)malloc(sizeof(hashNode) * this->tableLength);
    log->debug("Hash closed map table length %d\n", tableLength);
}

bool
hashMapClosed::isPrime (uint32_t num) 
{
    int sq_root = sqrt(num);
    for(int i = 2; i <= sq_root; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

uint32_t
hashMapClosed::getNextPrime (uint32_t num) 
{
    while (!isPrime(num)) {
        num++;
    }
    return num;
}

hashMapClosed::hashMapClosed (int tableLength, hashClosedEnum hashtype, 
                              bool rehash, logger *log):
    hashMap(tableLength, log)
{
    this->hashType = hashtype;
    this->rehash = rehash;
    this->tableLength = getNextPrime(this->tableLength);
    tableInit();
}
hashMapClosed::hashMapClosed (int tableLength, bool rehash, logger *log):
    hashMap(tableLength, log)
{
    this->hashType = HASH_CLOSED_LINEAR;
    this->rehash = rehash;
    this->tableLength = getNextPrime(this->tableLength);
    tableInit();
}

hashMapClosed::hashMapClosed (bool rehash, logger *log):
    hashMap(log)
{
    this->hashType = HASH_CLOSED_LINEAR;
    this->rehash = rehash;
    this->tableLength = getNextPrime(this->tableLength);
    tableInit();
}

apiRetVal
hashMapClosed::walk (walkcbk cbk)
{
    uint32_t iter = 0;
    hashNode *node = NULL;

    if (!numberOfElements) {
        return API_RETVAL_SUCCESS;
    }

    while (iter < tableLength) {
        node = &this->hashTable[iter];
        if (node->nodeState & HASH_NODE_USED) {
            if (cbk(&node->nodeKey, node->data) != CBK_RET_CONTINUE) {
                return API_RETVAL_SUCCESS;
            }
        }
        iter++;
    }
    return API_RETVAL_SUCCESS;
}

hashMapClosed::~hashMapClosed ()
{
    if (fcbk) {
        walk(fcbk);
    }
    free(hashTable);
}

void
hashMapClosed :: rehashTable (void)
{
    uint32_t iter = 0, oldTableLength; 
    hashNode *node = NULL;
    hashNode *oldHashTable = NULL;

    if (!numberOfElements) {
        return;
    }
    log->info("Rehashing started\n");

    oldTableLength = tableLength;
    oldHashTable = this->hashTable; 
    
    this->numberOfElements = 0;
    this->hashTable = NULL;
    this->tableLength = getNextPrime(tableLength*2);
    tableInit();

    while (iter < oldTableLength) {
        node = &oldHashTable[iter];
        if (node->nodeState & HASH_NODE_USED) {
            add(&node->nodeKey, node->data);
        }
        iter++;
    }
    log->info("Rehashing Done\n");
    free(oldHashTable);
}



apiRetVal hashMapClosed::add (hashNodeKey *key, void *data)
{
    uint32_t position;
    hashNode *node = NULL;

    if (!data || !key) {
        log->error("Null data while adding\n");
        return (API_RETVAL_INVALID_INPUT);
    }

    position = getPosition(key, HASH_NODE_EMPTY | 
                                HASH_NODE_DELETED); 

    if (position >= tableLength) {
        if (rehash) {
        } else {
            log->error("No space left in the table\n");
            return (API_RETVAL_FAILED);
        }
    }
    node = &hashTable[position];

    node->data = data;
    node->nodeKey = *key;
    node->nodeState = HASH_NODE_USED;
    numberOfElements++;

    if (rehash && (numberOfElements * 2 > tableLength)) {
        rehashTable();
    }

    log->info("Data added to hash table\n");
    return (API_RETVAL_SUCCESS);
}

apiRetVal hashMapClosed::remove (hashNodeKey *key, void **data)
{
    uint32_t position;
    hashNode *node = NULL;

    if (!data || !key) {
        log->error("Null data while removing\n");
        return (API_RETVAL_INVALID_INPUT);
    }

    position = getPosition(key, HASH_NODE_USED);

    if (position >= tableLength) {
        return (API_RETVAL_DATA_NOT_FOUND);
    }
    node = &hashTable[position];

    *data = node->data; 
    node->nodeKey = *key;
    node->nodeState = HASH_NODE_DELETED;
    node->data = NULL;

    log->info("Data removed from hash table\n");
    return (API_RETVAL_SUCCESS);
}

apiRetVal hashMapClosed::find (hashNodeKey *key, void **data)
{
    uint32_t position;
    hashNode *node = NULL;

    if (!data || !key) {
        log->error("Null data while removing\n");
        return (API_RETVAL_INVALID_INPUT);
    }

    position = getPosition(key, HASH_NODE_USED);

    if (position >= tableLength) {
        return (API_RETVAL_DATA_NOT_FOUND);
    }
    node = &hashTable[position];

    *data = node->data; 
    node->nodeKey = *key;

    log->info("Data found in hash table\n");
    return (API_RETVAL_SUCCESS);
}
