#include <iostream>
#include <string>
#include <fstream>
#include "hashMap.h"
#include "hashMapOpen.h"
#include <string.h>


using namespace std;

int main (int argc, char *argv[])
{
    int nameLen;
    ifstream inFile;
    string fileName = "./intKeyValue.txt";
    char line[256], *name = NULL;
    uint32_t slNo;
    hashNodeKey key;
    logger *log = new logger(argv[0], stdout);
    log->setDebug(true);
    log->setVerbose(true);

    hashMap *hash = new hashMapOpen(10, HASHING_SIMPLE_MOD_INTEGER, log);

    cout << "Enter filename to read from: ";
    cin >> fileName;

    inFile.open(fileName.data());

    if (inFile.fail()) {
        cout <<"\nFailed to open file "<<fileName;
        return 1;
    }

    while (inFile.getline(line, 256, ',')) {

        slNo = atoi(line);
        cout << slNo << " - ";
        inFile.getline(line, 256);
        nameLen = strlen(line)+1;
        name = (char *) malloc(nameLen);
        memcpy(name, line, nameLen);
        cout << name << "\n";

        key.keyType = HASH_NODE_KEY_INT;
        key.keyByteLength = sizeof(uint32_t);
        key.intKey = slNo;
        hash->add(&key, name);
    }


    inFile.close();

    return 0;
}
