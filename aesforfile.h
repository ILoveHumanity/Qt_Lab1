#ifndef AESFORFILE_H
#define AESFORFILE_H

#include "ICypherMetod.h"

/// @brief Шифрование/дешифрование файлов на основе AES-256-GCM
class AESForFile : public ICypherMetod
{
public:
    /// @brief Деструктор
    ~AESForFile() = default;

    /// @brief Метод для получения доступа к единственному экземпляру
    /// @return Ссылка на экземпляр AESForFile
    static AESForFile& getInstance();

    /// @brief Шифрование файла с помощью пароля
    /// @param[in] pathToFile Путь к файлу для шифрования
    /// @param[in] password Пароль для шифрования
    /// @return true при успешном шифровании, false в случае ошибки
    bool encryptFileWithPass(const QString& pathToFile, const QString& password);

    /// @brief Дешифрование файла с помощью пароля
    /// @param[in] pathToFile Путь к файлу для дешифрования
    /// @param[in] password Пароль для дешифрования
    /// @return true при успешном дешифровании, false в случае ошибки
    bool decryptFileWithPass(const QString& pathToFile, const QString& password);
private:
    /// @brief Приватный конструктор для паттерна Singleton
    AESForFile() = default;
    AESForFile(const AESForFile&) = delete; // запрещаем копирование
    AESForFile& operator=(const AESForFile&) = delete; // запрещаем присвоение
};

#endif // AESFORFILE_H
