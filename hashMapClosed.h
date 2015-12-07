#ifndef __HASH_MAP_CLOSED_H__
#define __HASH_MAP_CLOSED_H__
#include "hashMap.h"

typedef enum hashClosedEnum_ {
    HASH_CLOSED_LINEAR,
    HASH_CLOSED_QUADRATIC,
    HASH_CLOSED_DOUBLE,
} hashClosedEnum;

class hashMapClosed : public hashMap {
    public:
        hashMapClosed(int tableLength, hashClosedEnum hashtype, 
                      bool enableRehash, logger *log);
        hashMapClosed(int tableLength, bool enableRehash, logger *log);
        hashMapClosed(bool enableRehash, logger *log);
        ~hashMapClosed();
        apiRetVal walk(walkcbk cbk);
        apiRetVal find(hashNodeKey *key, void **data);
        apiRetVal add(hashNodeKey *key, void *data);
        apiRetVal remove(hashNodeKey *key, void **data);

    private:
        hashNode *hashTable;
        hashClosedEnum hashType;
        bool rehash;
        void tableInit();
        void rehashTable();
        bool isPrime(uint32_t num);
        uint32_t getNextPrime(uint32_t num);
        const char* NodeStateString(uint32_t nodeState);
        uint32_t getPosition(hashNodeKey *key, uint32_t nodeState);
        uint32_t getPositionLinearProbing(hashNodeKey *key, uint32_t nodeState);
        uint32_t getPositionQuadProbing(hashNodeKey *key, uint32_t nodeState);
        uint32_t getPositionDoubleProbing(hashNodeKey *key, uint32_t nodeState);
        void printTable();
};

#endif
