#ifndef ICYPHERMETOD_H
#define ICYPHERMETOD_H
#pragma once

#include <QString>

/// @brief Интерфейс для методов шифрования/дешифрования
class ICypherMetod
{
public:
    /// @brief Виртуальный деструктор
    virtual ~ICypherMetod() = default;

    /// @brief Шифрование файла с помощью пароля
    /// @param[in] pathToFile Путь к файлу для шифрования
    /// @param[in] password Пароль для шифрования
    /// @return true при успешном шифровании, false в случае ошибки
    virtual bool encryptFileWithPass(const QString& pathToFile, const QString& password) = 0;

    /// @brief Дешифрование файла с помощью пароля
    /// @param[in] pathToFile Путь к файлу для дешифрования
    /// @param[in] password Пароль для дешифрования
    /// @return true при успешном дешифровании, false в случае ошибки
    virtual bool decryptFileWithPass(const QString& pathToFile, const QString& password) = 0;
};

#endif // ICYPHERMETOD_H
