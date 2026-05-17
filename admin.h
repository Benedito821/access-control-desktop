#ifndef ADMIN_H
#define ADMIN_H

#include <QHttpServer>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QTimer>

enum class ACCESS_STATE
{
    DENIED,
    GRANTED,
    UNKNOWN
};

#define CON_STATE_UP "CONNECTED"
#define CON_STATE_DOWN "DISCONNECTED"
#define HTTP_PORT 3000

QT_BEGIN_NAMESPACE
namespace Ui
{
class admin;
}
QT_END_NAMESPACE

class admin : public QMainWindow
{
    Q_OBJECT

  public:
    admin(QWidget* parent = nullptr);
    ~admin();

  private:
    Ui::admin* ui = nullptr;

    QTimer* statusBarUpdateTimer = nullptr;

    quint16 currentPort = 0;

    QHttpServer httpServer;

    QSqlDatabase users_db;

    void setupDatabase();

    void setupHttpServer();

    void updateUi(ACCESS_STATE access, const QString& name, const QString& photo) const;

    void updatestatusBar() const;

    bool hasNetworkConnection() const;
};
#endif // ADMIN_H
