#include <QCoreApplication>
#include <EncDec.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QString path;
    QTextStream in(stdin);
    QTextStream out(stdout);
    out << "Enter path: " << Qt::flush;
    in >> path;
    EncDec a;
    a.print_all_in_dir(path);

    return 0;
    //return a.exec();
}
