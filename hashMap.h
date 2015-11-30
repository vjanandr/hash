#ifndef __HASHMAP__
#define __HASHMAP__

#include "hashUtil.h"

typedef enum cbkRetVal_ {
    CBK_RET_CONTINUE,
    CBK_RET_STOP,
    CBK_RET_EQUAL,
} cbkRetVal;

typedef cbkRetVal (*walkcbk)(void *data);

class hashMap {
    public:
        hashMap(int tableLength);
        hashMap();
        virtual apiRetVal walk(walkcbk cbk) = 0;
        virtual apiRetVal find(hashNodeKey *key, void **data) = 0;
        virtual apiRetVal add(hashNodeKey *key, void *data) = 0;
        virtual apiRetVal remove(hashNodeKey *key void **data) = 0;
        int getTableLength();
        int getNumberOfElements();

    protected:
        int tableLength;
        int numberOfElements;
};

#endif
