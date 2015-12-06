#include "hashMapClosed.h"
#define MAX_ITER 10

char *
hashMapClosed::NodeStateString (uint32_t nodeState)
{
    char str[1024];
    string *nodeStr = NULL;

    if (nodeState & HASH_NODE_USED) {
        nodeStr = "NODE_USED";
    }
    if (nodeState & HASH_NODE_EMPTY) {
        if (nodeStr) {
            nodeStr = nodeStr+":NODE_EMPTY";
        } else {
            nodeStr = "NODE_EMPTY";
        }
    }
    if (nodeState & HASH_NODE_DELETED) {
        if (nodeStr) {
            nodeStr = nodeStr+":NODE_DELETED";
        } else {
            nodeStr = "NODE_DELETED";
        }
    }
    return nodeStr;
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
                (keyCmp(key, &node->key))) {
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
    uint32_t itercnt = 0;
    uint32_t iter = 1;
    uint32_t position, hkey;

    hkey = gethashkey(key);
    position = hkey;
    while (true) {
        position = position % tablelength;
        node = &hashtable[position];

        if (nodestate & HASH_NODE_USED) {
            if ((node->nodestate & HASH_NODE_USED) &&
                (keycmp(key, &node->key))) {
                return position;
            }
            if (node->nodestate & HASH_NODE_EMPTY) {
                return (tablelength + 1);
            }
        } else if (nodestate & (HASH_NODE_EMPTY | HASH_NODE_DELETED)) {
            if (node->nodestate & nodestate) {
                return position;
            }
        } else {
            // something wrong;
            log->error("Invalid node state %d \n",nodeState);
            return (tablelength + 1);
        }
        if (itercnt > max_iter * tablelength) {
            log->error("max iter reached node not found \n");
            return (tablelength + 1);
        }
        itercnt++;
        position = (hkey + iter * iter);
        iter++;
    }
}

uint32_t 
hashMapClosed::getPositionDoubleProbing (hashNodeKey *key, uint32_t nodeState)
{
    uint32_t itercnt = 0;
    uint32_t iter = 1;
    uint32_t position, hkey1, hkey2;

    hkey = gethashkey(key);
    hkey2 = 7 - gethashkey(key, 7);
    if (!hkey2) {
        hkey2 = 1 + gethashkey(key, 7);
    }
    position = hkey;
    while (true) {
        position = position % tablelength;
        node = &hashtable[position];

        if (nodestate & HASH_NODE_USED) {
            if ((node->nodestate & HASH_NODE_USED) &&
                (keycmp(key, &node->key))) {
                return position;
            }
            if (node->nodestate & HASH_NODE_EMPTY) {
                return (tablelength + 1);
            }
        } else if (nodestate & (HASH_NODE_EMPTY | HASH_NODE_DELETED)) {
            if (node->nodestate & nodestate) {
                return position;
            }
        } else {
            // something wrong;
            log->error("Invalid node state %d \n",nodeState);
            return (tablelength + 1);
        }
        if (itercnt > max_iter * tablelength) {
            log->error("max iter reached node not found \n");
            return (tablelength + 1);
        }
        itercnt++;
        position = (hkey1 + iter * hkey2);
        iter++;
    }
}

hashMapClosed::tableInit()
{
    this->hashTable = (hashNode *)malloc(sizeof(hashNode) * this->tableLength);
    switch (hashType) {
        case HASH_CLOSED_LINEAR:
            getPosition = getPositionLinearProbing;
            break;
        case HASH_CLOSED_QUADRATIC:
            getPosition = getPositionQuadProbing;
            break;
        case HASH_CLOSED_DOUBLE:
            getPosition = getPositionDoubleProbing;
            break;
    }
    log->debug("Hash closed map table length %d\n", tableLength);
}

hashMapClosed::hashMapClosed (int tableLength, hashClosedEnum hashtype, 
                              bool rehash, logger *log):
    hashMap(tableLength, log)
{
    this->hashType = hashtype;
    this->rehash = rehash;
    tableInit();
}
hashMapClosed::hashMapClosed (int tableLength, bool rehash, logger *log):
    hashMap(tableLength, log)
{
    this->hashType = HASH_CLOSED_LINEAR;
    this->rehash = rehash;
    tableInit();
}

hashMapClosed::hashMapClosed (bool rehash, logger *log):
    hashMap(log)
{
    this->hashType = HASH_CLOSED_LINEAR;
    this->rehash = rehash;
    tableInit();
}

hashMapClosed::~hashMapClosed ()
{
    if (fcbk) {
        walk(fcbk);
    }
    free(hashTable);
}

apiRetVal hashMapClosed::add(hashNodeKey *key, void *data)
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

    log->info("Data added to hash table\n");
    return (API_RETVAL_SUCCESS);
}
