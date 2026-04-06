#include <QCoreApplication>
#include <EncDec.h>
#include <AESForFile.h>

#include <openssl/rand.h>
#include <openssl/opensslv.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    QString path;
    QTextStream in(stdin);
    QTextStream out(stdout);
    out << "Enter path: " << Qt::endl;
    path = in.readLine();
    AESForFile cypher;

    EncDec& EncDec_Instance = EncDec::getInstance(&cypher);
    EncDec_Instance.printAllInDir(path);
    //EncDec_Instance.encryptAllInDir(path, "qwerty");

//    if(!cypher.encryptFileWithPass(path, "qwerty"))
//    {
//        out << "Error" << Qt::flush;
//        return 1;
//    }

    unsigned char key[4] = {0,0};
    try {
        if(RAND_bytes(key, sizeof(key)) == 0){
            throw 1;
        }
    }
    catch (...) {
        out << "Error" << Qt::flush;
        return 1;
    }
    out << key[0] << " " << key[1] << Qt::flush;

    return 0;
    //return a.exec();
}
