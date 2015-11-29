#ifndef __HASH_UTIL_H__
#define __HASH_UTIL_H__

typedef enum compare_ret_e_ {
    COMPARE_RET_CONTINUE,
    COMPARE_RET_STOP,
    COMPARE_RET_EQUAL,
} compare_ret_e;

#define FOR_EACH_RETVAL_ENUM_GENERATOR(GENERATOR)              \
    GENERATOR(API_RETVAL_SUCCESS, "Api returned success")      \
    GENERATOR(API_RETVAL_FAILED, "Api returned failure")       

#define API_ENUM_GENERATOR(ENUM,STRING) ENUM,

typedef enum api_ret_e_ {
    FOR_EACH_RETVAL_ENUM_GENERATOR(API_ENUM_GENERATOR)
} api_ret_e;

extern const char* api_ret_get_string(api_ret_e reval);

#endif
