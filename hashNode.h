#ifndef __HASH_NODE_H__
#define __HASH_NODE_H__

typedef enum hashNodeState_ {
    HASH_NODE_EMPTY,
    HASH_NODE_USED,
    HASH_NODE_DELETED
} hashNodeState;

typedef enum hashNodeKeyType_ {
    HASH_NODE_KEY_STRING,
    HASH_NODE_KEY_INT,
    HASH_NODE_KEY_BYTE
} hashNodeKeyType;

typedef struct hashNodeKey_ {
    hashNodeKeyType keyType;
    uint32_t keyByteLength;
    union {
        uint32_t intKey;
        char *strKey;
        uint8_t *byteKey;
    };
} hashNodeKey;

typedef struct hashNode_ {
    struct hashNode_ *next;
    void *data;
    hashNodeKey nodeKey;
    hashNodeState nodeState;
} hashNode;

#endif
