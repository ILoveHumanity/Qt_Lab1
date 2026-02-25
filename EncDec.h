#ifndef ENCDEC_H
#define ENCDEC_H
#pragma once
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <qDebug>
#include <QFileInfo>

class EncDec
{
public:
    EncDec();
    ~EncDec();
    void print_all_in_dir(const QString &);
    void Encrypt_all_in_dir(const QString &, const QString &);
    void Decrypt_all_in_dir(const QString &, const QString &);
};

#endif // ENCDEC_H
