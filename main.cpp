#include "admin.h"
#include <QIcon>

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/uploads/icon.png"));
    admin w;
    w.show();
    return app.exec();
}
