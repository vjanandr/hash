#ifndef __HASHMAP__
#define __HASHMAP__

#include "hashUtil.h"

typedef compareRetVal (*comparecbk)(void *data1, void *data2);

class hashMap {
    public:
        hashMap(int tableLength);
        hashMap();
        virtual apiRetVal walk(comparecbk cbk) = 0;
        virtual apiRetVal find(void *data, comparecbk cbk) = 0;
        virtual apiRetVal add(void *data, comparecbk cbk) = 0;
        virtual apiRetVal remove(void *data, comparecbk cbk) = 0;
        int getTableLength();
        int getNumberOfElements();

    protected:
        int tableLength;
        int numberOfElements;
};

#endif
