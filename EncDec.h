#ifndef ENCDEC_H
#define ENCDEC_H
#pragma once
#include <ICypherMetod.h>
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <qDebug>
#include <QFileInfo>

class EncDec
{
    ICypherMetod* cypher;
private:
    EncDec(); // приватный конструктор для гарантии одного обьекта
    EncDec(const EncDec&) = delete; // запрещаем копирование
    EncDec& operator=(const EncDec&) = delete; // запрещаем присвоение
public:
    ~EncDec();
    static EncDec& getInstance(ICypherMetod* cypher_); // метод получения доступа к обьекту
    void setCypher(ICypherMetod* cypher_);
    void printAllInDir(const QString &);
    void encryptAllInDir(const QString &, const QString &);
    void decryptAllInDir(const QString &, const QString &);
};

#endif // ENCDEC_H
