#ifndef __HASH_OPEN_H__
#define __HASH_OPEN_H__

#include "hashMap.h"
#include "hashNode.h"

typedef enum hashEnum_ {
    HASHING_SIMPLE_MOD_INTEGER,
    HASHING_SIMPLE_MOD_STRING,
    HASHING_CRC
} hashEnum;

class hashMapOpen: public hashMap
{
    public:
        hashMapOpen(int tableLength, hashEnum hashtype, logger *log);
        hashMapOpen(int tableLength, logger *log);
        hashMapOpen(logger *log);
        ~hashMapOpen();
        apiRetVal walk(walkcbk cbk);
        apiRetVal find(hashNodeKey *key, void **data);
        apiRetVal add(hashNodeKey *key, void *data);
        apiRetVal remove(hashNodeKey *key, void **data);

    private:
        hashNode **hashTable;
        hashEnum hashType;
        uint32_t hashSimpleModInteger(uint32_t key);
        uint32_t hashSimpleModString(char *str, uint32_t bytelength);
        uint32_t hashCRC(uint8_t *bytes, uint32_t len);
        void tableInit();
        uint32_t getHashKey(hashNodeKey *nodeKey);
};

#endif
