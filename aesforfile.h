#ifndef AESFORFILE_H
#define AESFORFILE_H

#include "ICypherMetod.h"

class AESForFile : public ICypherMetod
{
public:
    AESForFile();
    ~AESForFile();
    bool encryptFileWithPass(const QString& pathToFile, const QString& password);
    bool decryptFileWithPass(const QString& pathToFile, const QString& password);
};

#endif // AESFORFILE_H
