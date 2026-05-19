#include "admin.h"
#include <QIcon>

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setDesktopFileName("access-control-admin");
    app.setApplicationName("access-control-admin");
    app.setWindowIcon(QIcon(":/uploads/icon.png"));
    admin w;
    w.show();
    return app.exec();
}
