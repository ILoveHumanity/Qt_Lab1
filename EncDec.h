#ifndef ENCDEC_H
#define ENCDEC_H
#pragma once
#include <ICypherMetod.h>
#include <QString>

/// @brief Singleton-класс для рекурсивного шифрования/дешифрования файлов в директории
class EncDec
{
public:
    /// @brief Деструктор
    ~EncDec() = default;

    /// @brief Метод для получения доступа к единственному экземпляру
    /// @return Ссылка на экземпляр EncDec
    static EncDec& getInstance();

    /// @brief Установить реализацию шифрования
    /// @param[in] cypher Указатель на реализацию шифрования
    void setCypher(ICypherMetod* cypher);

    /// @brief Вывести все файлы подверженные обработке в директории
    /// @param[in] path Путь к директории
    void printAllInDir(const QString &);

    /// @brief Рекурсивно зашифровать все файлы в директории
    /// @param[in] path Путь к директории
    /// @param[in] password Пароль для шифрования
    void encryptAllInDir(const QString &, const QString &);

    /// @brief Рекурсивно дешифровать все файлы в директории
    /// @param[in] path Путь к директории
    /// @param[in] password Пароль для шифрования
    void decryptAllInDir(const QString &, const QString &);

private:
    /// @brief Приватный конструктор для паттерна Singleton
    EncDec();
    EncDec(const EncDec&) = delete; // запрещаем копирование
    EncDec& operator=(const EncDec&) = delete; // запрещаем присвоение

    /// @brief Проверка возможности работы с файлом
    /// @param[in] path Путь к файлу
    bool testFile(const QString& filePath);

private:
    ICypherMetod* cypher_; ///< Указатель на реализацию шифрования (не владеет)
};

#endif // ENCDEC_H
