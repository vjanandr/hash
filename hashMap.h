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

class hashMap {
    public:
        hashMap(int tableLength, logger *log);
        hashMap(logger *log);
        virtual apiRetVal walk(walkcbk cbk) = 0;
        virtual apiRetVal find(hashNodeKey *key, void **data) = 0;
        virtual apiRetVal add(hashNodeKey *key, void *data) = 0;
        virtual apiRetVal remove(hashNodeKey *key, void **data) = 0;
        int getTableLength();
        int getNumberOfElements();

    protected:
        logger *log;
        uint32_t tableLength;
        uint32_t numberOfElements;
        bool keyCmp(hashNodeKey *key1, hashNodeKey *key2);
};

#endif
