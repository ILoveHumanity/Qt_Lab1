#ifndef ENCDEC_H
#define ENCDEC_H
#pragma once
#include <ICypherMetod.h>
#include <QString>

class EncDec
{
public:
    ~EncDec() = default;
    static EncDec& getInstance(ICypherMetod* cypher); // метод получения доступа к обьекту
    void setCypher(ICypherMetod* cypher);
    void printAllInDir(const QString &);
    void encryptAllInDir(const QString &, const QString &);
    void decryptAllInDir(const QString &, const QString &);

private:
    EncDec(); // приватный конструктор для гарантии одного обьекта
    EncDec(const EncDec&) = delete; // запрещаем копирование
    EncDec& operator=(const EncDec&) = delete; // запрещаем присвоение

private:
    ICypherMetod* cypher_;
};

#endif // ENCDEC_H
