#ifndef __HASHMAP__
#define __HASHMAP__

#include "hash_util.h"

typedef compare_ret_e (*comparecbk)(void *data1, void *data2);

class hashMap {
    public:
        hashMap(int table_length);
        hashMap();
        virtual api_ret_e walk(comparecbk cbk) = 0;
        virtual api_ret_e find(void *data, comparecbk cbk) = 0;
        virtual api_ret_e add(void *data, comparecbk cbk) = 0;
        virtual api_ret_e remove(void *data, comparecbk cbk) = 0;
        int getTableLength();
        int getNumberOfElements();

    protected:
        int table_length;
        int number_of_elements;
};

#endif
