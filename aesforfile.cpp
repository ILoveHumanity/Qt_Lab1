#include "aesforfile.h"
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <QSaveFile>
#include <QFile>
#include <qDebug>

QByteArray EncryptedFlag = "727780ee7239ad2c3fbf50969a64f852"; // EncryptedFlag на EncryptedFlag с помощью MD5

AESForFile::AESForFile()
{

}
AESForFile::~AESForFile()
{

}
bool encryptFileWithPass(const QString& pathToFile, const QString& password)
{
    QFile inputFile(pathToFile);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open input file: " << pathToFile;
        return false;
    }

    const QByteArray fileEncryptedFlag = inputFile.read(EncryptedFlag.size());
    if (fileEncryptedFlag == EncryptedFlag)
    {
        qDebug() << "File is already encrypted: " << pathToFile;
        return false;
    }

    if (!inputFile.seek(0))
    {
        qDebug() << "Failed to seek input file: " << pathToFile;
        return false;
    }

    QSaveFile outputFile(pathToFile);
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open output file: " << pathToFile;
        return false;
    }

    return true;
}
bool decryptFileWithPass(QString& pathToFile, QString& password)
{
    return true;
}
