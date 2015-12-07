#include <iostream>
#include <string>
#include <fstream>
#include "hashMap.h"
#include "hashMapClosed.h"
#include <string.h>


using namespace std;

cbkRetVal
walkCbkFunc (hashNodeKey *key, void *data)
{
    uint32_t *count = (uint32_t *)data;
    printf("\n%-10d | %s", *count,key->strKey) ;
    return CBK_RET_CONTINUE;
}

int main (int argc, char *argv[])
{
    uint32_t *count = NULL;
    ifstream inFile;
    string fileName;
    char *word= NULL, wordtmp[256], *wordtmp2;
    int wordlen;
    hashNodeKey key;
    logger *log = new logger(argv[0], stdout);
    log->setDebug(true);
    log->setVerbose(true);
    log->setError(true);
    log->setInfo(true);

    hashMap *hash = new hashMapClosed(10, HASH_CLOSED_LINEAR, true, log);

    if (argc > 1) {
        inFile.open(argv[1]);
    } else {
        cout << "Enter filename to read from: ";
        cin >> fileName; 
        inFile.open(fileName.data());
    }

    if (inFile.fail()) {
        cout <<"\nFailed to open file "<<fileName;
        return 1;
    }

    while (inFile.getline(wordtmp, 256, ' ')) {
        wordlen = strlen(wordtmp);
        wordtmp2 = wordtmp;
        if (wordtmp[wordlen-1] == '\n' ||
            wordtmp[wordlen-1] == ',' ||
            wordtmp[wordlen-1] == '.') {
            wordtmp[wordlen-1] = '\0';
            wordlen = wordlen -1;
        }

        if (wordtmp[0] == '\n' ||
            wordtmp[0] == ',' ||
            wordtmp[0] == '.') {
            wordtmp2 = wordtmp + 1;
            wordlen = wordlen -1;
        }
        key.keyType = HASH_NODE_KEY_BYTE;
        key.keyByteLength = wordlen+1;
        key.strKey = wordtmp2;
        count = NULL;
        if (hash->find(&key, (void **)&count) == API_RETVAL_SUCCESS) {
            if (count) {
                log->info("increment count %d %p\n",*count);
                *count = *count +1;
            } 
        } else {
            word = (char *) malloc(key.keyByteLength);
            memcpy(word, wordtmp2, key.keyByteLength);
            key.strKey = word;
            count = (uint32_t *) malloc(sizeof(uint32_t));
            *count = 1;
            hash->add(&key, (void *)count);
        }
    }
    inFile.close();

    printf("\n%-10s | Word","Count");
    printf("\n-------------------------");
    hash->walk(walkCbkFunc);
    cout << "\n";
    return 0;
}
