#include "encdec.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>
#include <windows.h>

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
bool EncDec::testFile(const QString& filePath){
    QFileInfo info(filePath);
    const DWORD attributes = GetFileAttributesW(reinterpret_cast<const wchar_t*>(info.absoluteFilePath().utf16()));
    return info.isFile() && !info.isSymLink() && attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_SYSTEM) == 0;
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

    while (dir_it.hasNext())
    {
        file_path = dir_it.next();
        // Пропуск символических ссылок для безопасности
        if(testFile(file_path))
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
    while (dir_it.hasNext()) {
        file_path = dir_it.next();
        // Пропуск символических ссылок для безопасности
        if(testFile(file_path)){
            if(!cypher_->decryptFileWithPass(file_path, password))
            {
                qDebug() << "Error in decryption of " + file_path;
            }
        }
    }
}
