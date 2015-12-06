#include "hashMapClosed.h"

uint32_t
hashMapClosed::getPositionLinearProbing (hashNodeKey *key, uint32_t nodeState)
{
    hashNode *node = NULL;
    uint32_t iter = 1;
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
            return (tableLength + 1);
        }
        position = (hkey + iter++);
    }
}

uint32_t
hashMapClosed::getPositionQuadProbing (hashNodeKey *key, uint32_t nodeState)
{
    uint32_t step = 1;
    uint32_t wrapped = false;
    uint32_t iter = 0;
    uint32_t position, hkey;

    hkey = getHashKey(key);
    position = hkey;
    while (true) {
        step++;
        if (hashTable[position].nodeState == nodeState ||
            hashTable[position].nodeState & nodeState) {
            return position;
        }
        position = hkey + step * step;
        if (position >= tableLength) {
            if (wrapped) {
                return tableLength + 1;
            }
            wrapped = true;
            position = 0;
        }
    }
}

uint32_t 
hashMapClosed::getPositionDoubleProbing (hashNodeKey *key, uint32_t nodeState)
{
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
