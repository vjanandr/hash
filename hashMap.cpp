#include "hashMap.h"
#include <string.h>

#define HASHMAP_DEF_TABLE_LENGTH 100

hashMap::hashMap(logger *log)
{
    this->log = log;
    this->tableLength = HASHMAP_DEF_TABLE_LENGTH;
    this->numberOfElements = 0;
    log->verbose("Hash Map created with table length %d \n",tableLength);
}

hashMap::hashMap (int tableLength, logger *log)
{
    this->log = log;
    this->tableLength = tableLength;
    this->numberOfElements = 0;
    log->verbose("Hash Map created with table length %d \n",tableLength);
}

int hashMap::getTableLength()
{
    return (this->tableLength);
}

int hashMap::getNumberOfElements()
{
    return (this->numberOfElements);
}

bool hashMap::keyCmp (hashNodeKey *key1, hashNodeKey *key2)
{
    bool keyCmp = false;
    if (!key1 || !key2) {
        log->error("Invalid input key1 %p, key2 %p\n", key1, key2);
        return keyCmp;
    }

    if (key1->keyType != key2->keyType) {
        log->verbose("KeyType din't match key1 type %d key2 type %d\n",
                      key1->keyType, key2->keyType);
        return keyCmp;
    }

    if (key1->keyByteLength != key2->keyByteLength) {
        log->verbose("Keybyte length dint match keyByteLength1 %d, "
                "keyByteLength2 %d", key1->keyByteLength, key2->keyByteLength);
        return keyCmp;
    }

    switch (key1->keyType) {
        case HASH_NODE_KEY_INT:
            keyCmp = (key1->intKey == key2->intKey);
        case HASH_NODE_KEY_STRING:
            keyCmp =  (!strncmp(key1->strKey, key2->strKey, 
                                key1->keyByteLength));
            break;
        case HASH_NODE_KEY_BYTE:
            keyCmp = (!memcmp(key1->byteKey, key2->byteKey,
                              key1->keyByteLength));
    }
    log->debug("Keys match %s type %d\n", keyCmp ? "Passed":"Failed",
            key1->keyType);
    return keyCmp;
}

