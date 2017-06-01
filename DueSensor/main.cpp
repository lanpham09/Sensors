#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("Omni Arm 1");
    w.show();

    return a.exec();
}
