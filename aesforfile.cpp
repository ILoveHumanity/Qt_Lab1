#include "aesforfile.h"

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <QFile>
#include <QSaveFile>
#include <QByteArray>
#include <QDebug>

QByteArray EncryptedFlag = "727780ee7239ad2c3fbf50969a64f852"; ///< Заголовок зашифрованного файла (MD5 от "EncryptedFlag")
int saltSize = 16; ///< Размер случайной соли для получения ключа из пароля в байтах
int aesKeySize = 32; ///< Размер ключа AES-256 в байтах (256 бит)
int iterationCount = 10000; ///< Количество итераций PBKDF2
int IVSize = 16; ///< Размер вектора инициализации в байтах
int tagSize = 16; ///< Размер тега аутентификации GCM в байтах

AESForFile& AESForFile::getInstance()
{
    static AESForFile s;
    return s;
}

bool AESForFile::encryptFileWithPass(const QString& pathToFile, const QString& password)
{
    // Открытие входного файла для чтения
    QFile inputFile(pathToFile);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open input file: " << pathToFile;
        return false;
    }
    // Чтение содержимого файла
    QByteArray fileContent = inputFile.readAll();
    if (inputFile.error() == QFileDevice::ReadError)
    {
        qDebug() << "Failed to read input file: " << pathToFile;
        inputFile.close();
        return false;
    }
    inputFile.close();

    // Пустые файлы пропускаются
    if (fileContent.isEmpty())
    {
        return true;
    }

    // Проверка, не зашифрован ли файл
    if (fileContent.size() > EncryptedFlag.size() + saltSize + IVSize + tagSize && fileContent.startsWith(EncryptedFlag))
    {
        qDebug() << "File is already encrypted: " << pathToFile;
        return false;
    }

    // Генерация случайной соли для получения ключа из пароля
    QByteArray salt(saltSize, 0);
    if (!RAND_bytes(reinterpret_cast<unsigned char*>(salt.data()), salt.size()))
    {
        qDebug() << "Failed to generate salt for encryption";
        return false;
    }

    // Получение ключа из пароля с помощью PBKDF2
    QByteArray encryptionKey(aesKeySize, 0);
    int deriveRet = PKCS5_PBKDF2_HMAC(password.toUtf8().constData(), password.toUtf8().size(),
                                      reinterpret_cast<const unsigned char*>(salt.constData()),
                                      salt.size(),iterationCount, EVP_sha256(), encryptionKey.size(),
                                      reinterpret_cast<unsigned char*>(encryptionKey.data()));
    if (deriveRet != 1)
    {
        qDebug() << "Failed to derive encryption key";
        return false;
    }

    // Генерация случайного IV
    QByteArray IV(IVSize, 0);
    if (!RAND_bytes(reinterpret_cast<unsigned char*>(IV.data()), IV.size()))
    {
        qDebug() << "Failed to generate nonce for encryption";
        return false;
    }

    // Создание контекста шифрования OpenSSL
    EVP_CIPHER_CTX* cipherContext = EVP_CIPHER_CTX_new();
    if (!cipherContext)
    {
        qDebug() << "Failed to allocate OpenSSL cipher context";
        return false;
    }

    // Инициализация AES-256-GCM шифрования
    if (!EVP_EncryptInit_ex(cipherContext, EVP_aes_256_gcm(), NULL, NULL, NULL) ||
        !EVP_CIPHER_CTX_ctrl(cipherContext, EVP_CTRL_GCM_SET_IVLEN, IV.size(), NULL) ||
        !EVP_EncryptInit_ex(cipherContext, NULL, NULL,
                            reinterpret_cast<const unsigned char*>(encryptionKey.constData()),
                            reinterpret_cast<const unsigned char*>(IV.constData()))
        )
    {
        qDebug() << "Failed to initialize AES-256-GCM encryption context";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    // Шифрование данных
    QByteArray encryptedFileContent(fileContent.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int outputLength = 0;

    if (!EVP_EncryptUpdate(cipherContext, reinterpret_cast<unsigned char*>(encryptedFileContent.data()), &outputLength,
                           reinterpret_cast<const unsigned char*>(fileContent.constData()), fileContent.size()))
    {
        qDebug() << "Failed to encrypt data";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    encryptedFileContent.truncate(outputLength);

    // Завершаем шифрование
    QByteArray finalChunk(EVP_MAX_BLOCK_LENGTH, 0);
    int finalLength = 0;

    if (!EVP_EncryptFinal_ex(cipherContext, reinterpret_cast<unsigned char*>(finalChunk.data()), &finalLength))
    {
        qDebug() << "Failed to encrypt data";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }
    if (finalLength > 0)
    {
        encryptedFileContent.append(finalChunk.left(finalLength));
    }

    // Открываем выходной файл на запись
    QSaveFile outputFile(pathToFile);
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open output file: " << pathToFile;
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    // Записываем флаг + соль + IV + шифротекст
    if (outputFile.write(EncryptedFlag) != EncryptedFlag.size() ||
        outputFile.write(salt) != salt.size() ||
        outputFile.write(IV) != IV.size() ||
        outputFile.write(encryptedFileContent) != encryptedFileContent.size())
    {
        qDebug() << "Failed to write encrypt data";
        outputFile.cancelWriting();
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    // Получение и запись тега аутентификации
    QByteArray tag(tagSize, 0);
    if (!EVP_CIPHER_CTX_ctrl(cipherContext, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data()) ||
        outputFile.write(tag) != tag.size() || !outputFile.commit())
    {
        qDebug() << "Failed to finalize encrypted file";
        outputFile.cancelWriting();
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }
    EVP_CIPHER_CTX_free(cipherContext);
    return true;
}

bool AESForFile::decryptFileWithPass(const QString& pathToFile, const QString& password)
{
    // Открытие входного файла для чтения
    QFile inputFile(pathToFile);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open input file: " << pathToFile;
        return false;
    }

    // Чтение содержимого файла
    QByteArray fileContent = inputFile.readAll();
    if (inputFile.error() == QFileDevice::ReadError)
    {
        qDebug() << "Failed to read input file: " << pathToFile;
        inputFile.close();
        return false;
    }
    inputFile.close();

    // Пустые файлы пропускаются
    if (fileContent.isEmpty())
    {
        return true;
    }

    // Проверка минимального размера зашифрованного файла
    if (fileContent.size() < EncryptedFlag.size() + saltSize + IVSize + tagSize)
    {
        qDebug() << "File is not encrypted (size does not match): " << pathToFile;
        return false;
    }
    // Проверка совпадения флага шифрования
    if (!fileContent.startsWith(EncryptedFlag))
    {
        qDebug() << "File is not encrypted (EncryptedFlag does not match): " << pathToFile;
        return false;
    }

    // Извлечение компонентов из файла
    fileContent = fileContent.right(fileContent.size() - EncryptedFlag.size());

    QByteArray salt = fileContent.left(saltSize);
    fileContent = fileContent.right(fileContent.size() - saltSize);

    QByteArray IV = fileContent.left(IVSize);
    fileContent = fileContent.right(fileContent.size() - IVSize);

    QByteArray tag = fileContent.right(tagSize);
    fileContent.chop(tagSize);

    // Получение ключа из пароля с помощью PBKDF2
    QByteArray decryptionKey(aesKeySize, 0);
    int deriveRet = PKCS5_PBKDF2_HMAC(password.toUtf8().constData(), password.toUtf8().size(),
                                      reinterpret_cast<const unsigned char*>(salt.constData()),
                                      salt.size(),iterationCount, EVP_sha256(), decryptionKey.size(),
                                      reinterpret_cast<unsigned char*>(decryptionKey.data()));
    if (deriveRet != 1)
    {
        qDebug() << "Failed to derive decryption key";
        return false;
    }

    // Создание контекста дешифрования
    EVP_CIPHER_CTX* cipherContext = EVP_CIPHER_CTX_new();
    if (!cipherContext)
    {
        qDebug() << "Failed to allocate OpenSSL cipher context";
        return false;
    }
    // Инициализация AES-256-GCM дешифрования
    if (!EVP_DecryptInit_ex(cipherContext, EVP_aes_256_gcm(), NULL, NULL, NULL) ||
        !EVP_CIPHER_CTX_ctrl(cipherContext, EVP_CTRL_GCM_SET_IVLEN, IV.size(), NULL) ||
        !EVP_DecryptInit_ex(cipherContext, NULL, NULL,
                            reinterpret_cast<const unsigned char*>(decryptionKey.constData()),
                            reinterpret_cast<const unsigned char*>(IV.constData())) ||
        !EVP_CIPHER_CTX_ctrl(cipherContext, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data()))
    {
        qDebug() << "Failed to initialize AES-256-GCM decryption context";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    // Дешифрование данных
    QByteArray decryptedFileContent(fileContent.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int outputLength = 0;

    if (!EVP_DecryptUpdate(cipherContext, reinterpret_cast<unsigned char*>(decryptedFileContent.data()), &outputLength,
                           reinterpret_cast<const unsigned char*>(fileContent.constData()), fileContent.size()))
    {
        qDebug() << "Failed to decrypt data";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }

    decryptedFileContent.truncate(outputLength);

    // Проверка тега аутентификации и завершение дешифрования
    QByteArray finalChunk(EVP_MAX_BLOCK_LENGTH, 0);
    int finalLength = 0;
    if (!EVP_DecryptFinal_ex(cipherContext, reinterpret_cast<unsigned char*>(finalChunk.data()), &finalLength))
    {
        qDebug() << "Failed to decrypt final data";
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }
    if (finalLength > 0)
    {
        decryptedFileContent.append(finalChunk.left(finalLength));
    }

    // Открываем выходной файл на запись
    QSaveFile outputFile(pathToFile);
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open output file: " << pathToFile;
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }
    // Запись расшифрованного файла
    if (outputFile.write(decryptedFileContent) != decryptedFileContent.size() ||
        !outputFile.commit())
    {
        qDebug() << "Failed to write decrypted output file";
        outputFile.cancelWriting();
        EVP_CIPHER_CTX_free(cipherContext);
        return false;
    }
    EVP_CIPHER_CTX_free(cipherContext);
    return true;
}
