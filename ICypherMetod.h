#ifndef ICYPHERMETOD_H
#define ICYPHERMETOD_H
#pragma once

#include <QString>

class ICypherMetod
{
public:
    virtual ~ICypherMetod() = default;
    virtual bool encryptFileWithPass(const QString& pathToFile, const QString& password) = 0;
    virtual bool decryptFileWithPass(const QString& pathToFile, const QString& password) = 0;
};

#endif // ICYPHERMETOD_H
