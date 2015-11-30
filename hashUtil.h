#ifndef __HASH_UTIL_H__
#define __HASH_UTIL_H__

#include <stdint.h>
#include <stdlib.h>

#define FOR_EACH_RETVAL_ENUM_GENERATOR(GENERATOR)                   \
    GENERATOR(API_RETVAL_MIN, "Api Min retval")                     \
    GENERATOR(API_RETVAL_SUCCESS, "Api returned success")           \
    GENERATOR(API_RETVAL_INVALID_INPUT, "Invalid input to API")     \
    GENERATOR(API_RETVAL_FAILED, "Api returned failure")            \
    GENERATOR(API_RETVAL_DATA_NOT_FOUND, "Data not found")          \
    GENERATOR(API_RETVAL_MAX, "Api Max retval")                     \

#define API_ENUM_GENERATOR(ENUM,STRING) ENUM,

typedef enum apiRetVal_ {
    FOR_EACH_RETVAL_ENUM_GENERATOR(API_ENUM_GENERATOR)
} apiRetVal;

extern const char* apiRetGetString(apiRetVal retval);

#endif
