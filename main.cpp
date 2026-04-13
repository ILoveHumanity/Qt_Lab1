#include <QCoreApplication>
#include <EncDec.h>
#include <AESForFile.h>
#include <QTextStream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    QString path;
    QTextStream in(stdin);
    QTextStream out(stdout);
//    out << "Enter path: " << Qt::flush;
//    path = in.readLine();
    path = "C:/repos/RSZI/Lab1/test";

    AESForFile cypher;
    EncDec& EncDec_Instance = EncDec::getInstance(&cypher);
    EncDec_Instance.printAllInDir(path);
    EncDec_Instance.encryptAllInDir(path, "qwerty");
    EncDec_Instance.decryptAllInDir(path, "qwerty");

    return 0;
    //return a.exec();
}
