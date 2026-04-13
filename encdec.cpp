#include "encdec.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>

EncDec::EncDec() : cypher_(nullptr)
{
}

EncDec& EncDec::getInstance(ICypherMetod* cypher)
{
    static EncDec s;
    s.setCypher(cypher);
    return s;
}
void EncDec::setCypher(ICypherMetod* cypher)
{
    cypher_ = cypher;
}
void EncDec::printAllInDir(const QString &path)
{
    QDir dir(path);
    if (dir.exists())
    {
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        while (dir_it.hasNext())
        {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(!info.isSymLink() && info.isFile())
            {
                qDebug() << file_path;
            }
        }
    }
    else
    {
        qDebug() << "Directory not found.";
    }
}

void EncDec::encryptAllInDir(const QString &path, const QString &password)
{
    if (!cypher_)
    {
        qDebug() << "Error сypher not set";
        return;
    }
    QDir dir(path);
    if (!dir.exists())
    {
        qDebug() << "Directory not found.";
        return;
    }

    QDirIterator dir_it(dir, QDirIterator::Subdirectories);
    QString file_path;
    QFileInfo info;
    while (dir_it.hasNext())
    {
        file_path = dir_it.next();
        info = dir_it.fileInfo();
        // Пропуск символических ссылок для безопасности
        if(!info.isSymLink() && info.isFile())
        {
            if(!cypher_->encryptFileWithPass(file_path, password))
            {
                qDebug() << "Error in encryption of " + file_path;
            }
        }
    }
}

void EncDec::decryptAllInDir(const QString &path, const QString &password)
{
    if (!cypher_)
    {
        qDebug() << "Error сypher not set";
            return;
    }
    QDir dir(path);
    if (!dir.exists())
    {
        qDebug() << "Directory not found.";
        return;
    }

    QDirIterator dir_it(dir, QDirIterator::Subdirectories);
    QString file_path;
    QFileInfo info;
    while (dir_it.hasNext()) {
        file_path = dir_it.next();
        info = dir_it.fileInfo();
        // Пропуск символических ссылок для безопасности
        if(!info.isSymLink() && info.isFile()){
            if(!cypher_->decryptFileWithPass(file_path, password))
            {
                qDebug() << "Error in decryption of " + file_path;
            }
        }
    }
}
