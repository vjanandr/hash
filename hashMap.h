#ifndef __HASHMAP__
#define __HASHMAP__

#include "hashUtil.h"
#include "hashNode.h"
#include "logger.h"

typedef enum cbkRetVal_ {
    CBK_RET_CONTINUE,
    CBK_RET_STOP,
    CBK_RET_EQUAL,
} cbkRetVal;

typedef cbkRetVal (*walkcbk)(hashNodeKey *key, void *data);
typedef walkcbk freecbk;

class hashMap {
    public:
        hashMap(int tableLength, logger *log);
        hashMap(logger *log);
        virtual apiRetVal walk(walkcbk cbk) = 0;
        virtual apiRetVal find(hashNodeKey *key, void **data) = 0;
        virtual apiRetVal add(hashNodeKey *key, void *data) = 0;
        virtual apiRetVal remove(hashNodeKey *key, void **data) = 0;
        void setFreeCbk(freecbk fcbk);
        int getTableLength();
        int getNumberOfElements();

    private:
        uint32_t hashSimpleModInteger(uint32_t key, uint32_t modLength);
        uint32_t hashSimpleModString(char *str, uint32_t bytelength, 
                                     uint32_t modLength);
        uint32_t hashCRC(uint8_t *bytes, uint32_t len, 
                         uint32_t modLength);

    protected:
        logger *log;
        freecbk fcbk;
        uint32_t tableLength;
        uint32_t numberOfElements;
        bool keyCmp(hashNodeKey *key1, hashNodeKey *key2);
        uint32_t getHashKey(hashNodeKey *nodeKey);
        uint32_t getHashKey(hashNodeKey *nodeKey, uint32_t modLength);
};

#endif
