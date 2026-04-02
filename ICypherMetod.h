#ifndef ICYPHERMETOD_H
#define ICYPHERMETOD_H
#pragma once

#include <QFileInfo>
#include <QVector>
#include <QString>

class ICypherMetod
{
public:
    virtual ~ICypherMetod() = default;
    virtual bool encryptFileWithPass(QString& pathToFile, QString& password) = 0;
    virtual bool decryptFileWithPass(QString& pathToFile, QString& password) = 0;
};

#endif // ICYPHERMETOD_H
