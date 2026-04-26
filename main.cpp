#include <QCoreApplication>
#include <EncDec.h>
#include <AESForFile.h>
#include <QTextStream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QTextStream in(stdin);
    QTextStream out(stdout);
    QString command;
    if (argc == 2) {
        command = argv[1];
    }
    else {
        out << "Enter command: " << Qt::flush;
        command = in.readLine();
    }

    QString path;
    out << "Enter path: " << Qt::flush;
    path = in.readLine();
    //    path = "C:/repos/RSZI/Lab1/test";

    out << "Enter password: " << Qt::flush;
    QString password = in.readLine();

    AESForFile cypher;
    EncDec& EncDec_Instance = EncDec::getInstance();
    EncDec_Instance.setCypher(&cypher);

    // Выполнение
    if (command == "list" || command == "ls") {
        EncDec_Instance.printAllInDir(path);
    }
    else if (command == "encrypt" || command == "enc") {
        EncDec_Instance.encryptAllInDir(path, password);
    }
    else if (command == "decrypt" || command == "dec") {
        EncDec_Instance.decryptAllInDir(path, password);
    }
    else {
        out << "Unknown command: " << command << Qt::endl;
        return 1;
    }
    out << "End" << Qt::endl;
    return 0;
    //return a.exec();
}
