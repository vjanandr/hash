#ifndef __LISH_H__
#define __LISH_H__

typedef struct listNode_ {
    struct listNode_ *next;
    void *data;
} listNode;

#endif
