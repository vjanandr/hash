#include "hashUtil.h"

#define API_STRING_GENERATOR(ENUM,STRING) STRING,

static const char* apiRetString[] = {
    FOR_EACH_RETVAL_ENUM_GENERATOR(API_STRING_GENERATOR)
};

const char* apiRetGetString (apiRetVal retval)
{
    if (retval < API_RETVAL_MIN || retval > API_RETVAL_MAX) {
        return ("Invalid api retval");
    }

    return apiRetString[retval];
}
