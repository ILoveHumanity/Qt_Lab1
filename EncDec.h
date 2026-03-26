#ifndef ENCDEC_H
#define ENCDEC_H
#pragma once
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <qDebug>
#include <QFileInfo>
#include <openssl/aes.h>

class EncDec
{
private:
    EncDec(); // приватный конструктор для гаранти одного обьекта
    EncDec(const EncDec&) = delete; // запрещаем копирование
    EncDec& operator=(const EncDec&) = delete; // запрещаем присвоение
public:
    ~EncDec();
    static EncDec& getInstance(); // метод получения доступа к обьекту
    void print_all_in_dir(const QString &);
    void Encrypt_all_in_dir(const QString &, const QString &);
    void Decrypt_all_in_dir(const QString &, const QString &);
};

#endif // ENCDEC_H
