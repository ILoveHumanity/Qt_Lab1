#include <QCoreApplication>
#include <EncDec.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QString path;
    QTextStream in(stdin);
    QTextStream out(stdout);
    out << "Enter path: " << Qt::endl;
    path = in.readLine();
    EncDec& EncDec_Instance = EncDec::getInstance();
    EncDec_Instance.print_all_in_dir(path);

    return 0;
    //return a.exec();
}
