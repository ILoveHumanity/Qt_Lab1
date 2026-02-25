#include "encdec.h"

EncDec::EncDec()
{

}
EncDec::~EncDec()
{

}
void EncDec::print_all_in_dir(const QString &path)
{
    QDir dir(path);
    if (dir.exists()){
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        while (dir_it.hasNext()) {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(info.isFile()){
                qDebug() << file_path << "\t" << (file_path == info.filePath());
            }
        }
    }
    else {
        qDebug() << "Directory not found.";
    }
}

void Encrypt_all_in_dir(const QString &path, const QString &password)
{
    QDir dir(path);
    if (dir.exists()){
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        // get key from password
        while (dir_it.hasNext()) {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(info.isFile()){
                //Encrypt with key
            }
        }
    }
    else {
        qDebug() << "Directory not found.";
    }
}

void Decrypt_all_in_dir(const QString &path, const QString &password)
{
    QDir dir(path);
    if (dir.exists()){
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        // get key from password
        while (dir_it.hasNext()) {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(info.isFile()){
                //Decrypt with key
            }
        }
    }
    else {
        qDebug() << "Directory not found.";
    }
}
