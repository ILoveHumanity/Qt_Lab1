#include <QCoreApplication>
#include <EncDec.h>
#include <openssl/err.h>
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
    EncDec& EncDec_Instance = EncDec::getInstance();
    EncDec_Instance.print_all_in_dir(path);

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
