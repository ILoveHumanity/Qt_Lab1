#include "encdec.h"

EncDec::EncDec()
{

}
EncDec::~EncDec()
{

}
EncDec& EncDec::getInstance(ICypherMetod* cypher_) {
    static EncDec s;
    s.setCypher(cypher_);
    return s;
}
void EncDec::setCypher(ICypherMetod* cypher_)
{
    cypher = cypher_;
}
void EncDec::printAllInDir(const QString &path)
{
    QDir dir(path);
    if (dir.exists()){
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        while (dir_it.hasNext()) {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(!info.isSymLink() && info.isFile()){
                qDebug() << file_path << "\t" << (file_path == info.filePath());
            }
        }
    }
    else {
        qDebug() << "Directory not found.";
    }
}

void EncDec::encryptAllInDir(const QString &path, const QString &password)
{
    QDir dir(path);
    if (dir.exists()){
        QDirIterator dir_it(dir, QDirIterator::Subdirectories);
        QString file_path;
        QFileInfo info;
        while (dir_it.hasNext()) {
            file_path = dir_it.next(); //dir_it.fileName();
            info = dir_it.fileInfo();
            if(!info.isSymLink() && info.isFile()){
                if(!cypher->encryptFileWithPass(file_path, password))
                {
                    qDebug() << "Error in encryption of " + file_path;
                }
            }
        }
    }
    else {
        qDebug() << "Directory not found.";
    }
}

void EncDec::decryptAllInDir(const QString &path, const QString &password)
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
