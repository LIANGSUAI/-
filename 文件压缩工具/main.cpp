#include "YourMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    YourMainWindow w;
    w.setWindowTitle("HuffmanZip");
    w.show();

    return a.exec();
}
