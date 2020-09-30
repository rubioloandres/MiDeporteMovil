#include "logregform.h"
#include "serverparams.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogRegForm w;

    w.show();

    return a.exec();
}
