#include "hash_util.h"

#define API_STRING_GENERATOR(ENUM,STRING) STRING,

static const char* api_ret_string[] = {
    FOR_EACH_RETVAL_ENUM_GENERATOR(API_STRING_GENERATOR)
};

const char* apiRetGetString (api_ret_e retval)
{
    if (retval < API_RETVAL_MIN || retval > API_RETVAL_MAX) {
        return ("Invalid api retval");
    }

    return api_ret_string[retval];
}
