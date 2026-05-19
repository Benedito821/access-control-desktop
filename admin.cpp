#include "admin.h"
#include "./ui_admin.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QPixmap>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrlQuery>

admin::admin(QWidget* parent) : QMainWindow(parent), ui(new Ui::admin)
{
    ui->setupUi(this);

    setupDatabase();

    setupHttpServer();

    updatestatusBar();

    updateUi(ACCESS_STATE::UNKNOWN, "UNKNOWN USER", ":/uploads/default.jpg");

    statusBarUpdateTimer = new QTimer(this);
    connect(statusBarUpdateTimer, &QTimer::timeout, this, &admin::updatestatusBar);
    statusBarUpdateTimer->start(1000);
}

admin::~admin() { delete ui; }

void admin::setupDatabase()
{
    users_db = QSqlDatabase::addDatabase("QSQLITE");

    QString users_db_path = QCoreApplication::applicationDirPath() + "/database/users";

    users_db.setDatabaseName(users_db_path);

    if (!users_db.open())
    {
        qDebug() << "Database error:" << users_db.lastError();
    }
    else
    {
        qDebug() << "Database successfully opened";
    }
}

void admin::updateUi(ACCESS_STATE access, const QString& name, const QString& photo) const
{
    switch (access)
    {
    case ACCESS_STATE::GRANTED:
        ui->accessStateLabel->setText("ACCESS GRANTED");
        ui->accessStateLabel->setStyleSheet("font-size: 32px; color: green;font-weight: bold;");
        break;
    case ACCESS_STATE::DENIED:
        ui->accessStateLabel->setText("ACCESS DENIED");
        ui->accessStateLabel->setStyleSheet("font-size: 32px; color: red;font-weight: bold;");
        break;
    default:
        ui->accessStateLabel->setText("UNKNOWN STATE");
        ui->accessStateLabel->setStyleSheet("font-size: 32px;font-weight: bold;");
        break;
    }

    ui->userInfoLabel->setText(name);

    QPixmap pix(photo);

    ui->photoLabel->setPixmap(
        pix.scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void admin::setupHttpServer()
{
    httpServer
        .route("/post", QHttpServerRequest::Method::Post,
               [this](const QHttpServerRequest& request)
               {
                   QString body = QString::fromUtf8(request.body());

                   QUrlQuery params(body);

                   QString uid = params.queryItemValue("Uid");

                   qDebug().noquote() << "Received uid:" << uid;

                   QSqlQuery query;

                   query.prepare("SELECT * FROM users WHERE uid = ?");

                   query.addBindValue(uid);

                   QJsonObject response;

                   QString name = "UNKNOWN USER";

                   int access = 0;

                   if (query.exec() && query.next())
                   {

                       access = query.value("allowed").toInt();

                       name = query.value("name").toString();

                       QString photo = query.value("photo").toString();

                       response["access"] = access;
                       response["name"] = name;
                       response["photo"] = photo;

                       QString photoPath = QCoreApplication::applicationDirPath() + "/" + photo;

                       updateUi(static_cast<ACCESS_STATE>(access), name, photoPath);
                   }
                   else
                   {

                       response["access"] = 0;
                       response["name"] = "UNKNOWN USER";
                       response["photo"] = ":/uploads/default.jpg";

                       updateUi(ACCESS_STATE::DENIED, "UNKNOWN USER", ":/uploads/default.jpg");
                   }

                   qDebug().noquote()
                       << QString("Name: %1 , Permission: %2").arg(name, QString::number(access));

                   QByteArray respBody = QJsonDocument(response).toJson(QJsonDocument::Compact);

                   return QHttpServerResponse("application/json", respBody);
               });

    const auto port = httpServer.listen(QHostAddress::Any, HTTP_PORT);

    currentPort = port;

    qDebug() << "HTTP server listening on port" << currentPort;
}

bool admin::hasNetworkConnection() const
{
    const auto interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface& iface : interfaces)
    {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            iface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            return true;
        }
    }

    return false;
}

void admin::updatestatusBar() const
{
    if (!currentPort || !hasNetworkConnection())
    {
        statusBar()->setStyleSheet("QStatusBar { background-color:#FFB6C1 ; color: black; }");
    }
    else
    {
        statusBar()->setStyleSheet("QStatusBar { background-color:#90EE90 ; color: black; }");
    }

    QString wifiState = hasNetworkConnection() ? CON_STATE_UP : CON_STATE_DOWN;

    QString httpServerState = currentPort ? CON_STATE_UP : CON_STATE_DOWN;

    QString msg = QString("SERVER: %1   |   WiFi: %2").arg(httpServerState, wifiState);

    statusBar()->showMessage(msg);
}
