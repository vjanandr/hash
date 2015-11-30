#ifndef __HASH_OPEN_H__
#define __HASH_OPEN_H__

#include "hashMap.h"
#include "listNode.h"

typedef enum hashEnum_ {
    HASHING_SIMPLE_MOD_INTEGER,
    HASHING_SIMPLE_MOD_STRING,
    HASHING_CRC
} hashEnum;

class hashMapOpen: public hashMap
{
    public:
        hashMapOpen(int tableLength, hashEnum hashtype);
        hashMapOpen(int tableLength);
        hashMapOpen();
        ~hashMapOpen();
        apiRetVal walk(walkcbk cbk);
        apiRetVal find(void *data, uint32_t byteLength, comparecbk cbk);
        apiRetVal add(void *data, uint32_t byteLength, comparecbk cbk);
        apiRetVal remove(void *data, uint32_t byteLength, comparecbk cbk);

    private:
        listNode **hashTable;
        hashEnum hashType;
        uint32_t hashSimpleModInteger(uint32_t key);
        uint32_t hashSimpleModString(char *str, uint32_t bytelength);
        uint32_t hashCRC(char *bytes, uint32_t len);
        void tableInit();
        uint32_t getHashKey(void *data, uint32_t byteLength);
};

#endif
