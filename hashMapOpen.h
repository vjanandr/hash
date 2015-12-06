#ifndef __HASH_OPEN_H__
#define __HASH_OPEN_H__

#include "hashMap.h"
#include "hashNode.h"

class hashMapOpen: public hashMap
{
    public:
        hashMapOpen(int tableLength, hashNodeKeyType hashtype, logger *log);
        hashMapOpen(int tableLength, logger *log);
        hashMapOpen(logger *log);
        ~hashMapOpen();
        apiRetVal walk(walkcbk cbk);
        apiRetVal find(hashNodeKey *key, void **data);
        apiRetVal add(hashNodeKey *key, void *data);
        apiRetVal remove(hashNodeKey *key, void **data);

    private:
        hashNode **hashTable;
        void tableInit();
};

#endif
