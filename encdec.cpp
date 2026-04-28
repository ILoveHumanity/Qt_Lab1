#include "encdec.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>
#include <windows.h>

EncDec::EncDec() : cypher_(nullptr)
{
}

EncDec& EncDec::getInstance()
{
    static EncDec s;
    return s;
}
void EncDec::setCypher(ICypherMetod* cypher)
{
    cypher_ = cypher;
}
void EncDec::printAllInDir(const QString &path)
{
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
        file_path = dir_it.next(); //dir_it.fileName();
        if(testFile(file_path))
        {
            qDebug() << file_path;
        }
    }
}
bool EncDec::testFile(const QString& filePath){
    QFileInfo info(filePath);
    const DWORD attributes = GetFileAttributesW(reinterpret_cast<const wchar_t*>(info.absoluteFilePath().utf16()));
    return info.isFile() && !info.isSymLink() && !info.isHidden() && attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_SYSTEM) == 0;
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

    QDirIterator dir_it(path, QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
    QString file_path;

    while (dir_it.hasNext())
    {
        file_path = dir_it.next();
        if(testFile(file_path))
        {
            if(cypher_->encryptFileWithPass(file_path, password))
            {
                qDebug() << "Successfully encrypted: " + file_path;
            }
        }
        else
        {
            qDebug() << "Skipped: " + file_path;
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

    QDirIterator dir_it(path, QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
    QString file_path;
    while (dir_it.hasNext()) {
        file_path = dir_it.next();
        if(testFile(file_path))
        {
            if(cypher_->decryptFileWithPass(file_path, password))
            {
                qDebug() << "Successfully decrypted: " + file_path;
            }
        }
        else
        {
            qDebug() << "Skipped: " + file_path;
        }
    }
}
