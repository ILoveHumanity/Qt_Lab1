#ifndef AESFORFILE_H
#define AESFORFILE_H

#include "ICypherMetod.h"

class AESForFile : public ICypherMetod
{
public:
    AESForFile();
    ~AESForFile();
    bool encryptFileWithPass(QString& pathToFile, QString& password) = 0;
    bool decryptFileWithPass(QString& pathToFile, QString& password) = 0;
};

#endif // AESFORFILE_H
