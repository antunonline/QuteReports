#include "quteapplication.h"
#include <QQmlEngine>
#include "qml/qmlsqlquerymodel.h"
#include "qml/qmlitemmodelxlsxexporter.h"
#include <QStringList>
#include <QDebug>
#include <QSqlError>
#include <QLocale>
#include <utility>

bool createSqlConnections(const QSettings & settings){
    if(settings.allKeys().contains("DB")==false){
        qDebug() << "Could not find \"DB\" settings key";
        return false;
    }

    auto sl = settings.value("DB").toStringList();
    for(auto &key : sl){
        QVariant driver, username, password, hostname, port, dbname;
        QString k = "DB/"+key;

        driver = settings.value(k+"/driver");
        username = settings.value(k+"/username");
        password = settings.value(k+"/password");
        hostname = settings.value(k+"/hostname");
        port = settings.value(k+"/port");
        dbname = settings.value(k+"/dbname");
        QVariant defaultConnection = settings.value(k+"/default");

        if(driver.type()==QVariant::Invalid ||
                username.type() == QVariant::Invalid ||
                password.type() == QVariant::Invalid ||
                hostname.type() == QVariant::Invalid ||
                port.type() == QVariant::Invalid ||
                dbname.type() == QVariant::Invalid) {
            qDebug() << "Not all required keys are specified for DB connection (one of: driver, username, password, hostname, port or dbname) " << k;
            continue;
        }

        QSqlDatabase d;
        if(defaultConnection.type() != QVariant::Invalid && defaultConnection.toBool())
            d = QSqlDatabase::addDatabase(driver.toString());
        else
            d = QSqlDatabase::addDatabase(driver.toString(), key);
        d.setHostName(hostname.toString());
        d.setUserName(username.toString());
        d.setPassword(password.toString());
        d.setPort(port.toInt());
        d.setDatabaseName(dbname.toString());

        if(d.open()==false){
            qDebug() << "Failed to open connection to " << k;
            qDebug() << d.lastError();
            continue;
        } else {
            qDebug() << "Database " << key << " opened";
        }
    }
    return true;
}

void QuteApplication::registerCustomQmlTypes()
{
    qmlRegisterType<QmlSqlQueryModel>("hr.com.unix", 1, 0, "SqlQueryModel");
    qmlRegisterType<QmlItemModelXlsxExporter>("hr.com.unix", 1, 0, "ItemModelXlsExporter");
}

QuteApplication::QuteApplication(int argc, char *argv[])
    : _qapp{argc, argv}, _reportEditorController{_reportModel}, _reportViewController{_reportModel}
{

}

void QuteApplication::bootstrap()
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    _settings = new QSettings("config.ini",  QSettings::IniFormat);
    createSqlConnections(*_settings);
    // _engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QLocale locale("hr_HR");


    bootstrapWindow();
}

void QuteApplication::bootstrapWindow()
{
    _mainWindow = std::unique_ptr<QuteMainWindow>(new QuteMainWindow(_reportEditorController));
    _mainWindow->setReportEditorDelegate(std::bind(&ReportEditController::showreport1Editor, _reportEditorController, std::placeholders::_1));
    _mainWindow->setReportWidgetBuilderDelegate(std::bind(&ReportViewController::createReportWidget, _reportViewController, std::placeholders::_1));
    _mainWindow->setReportLoaderDelegate(std::bind(&ReportModel::fetchById, _reportModel, std::placeholders::_1));
    _mainWindow->setDeleteReportDelegate(std::bind(&ReportModel::remove, _reportModel, std::placeholders::_1));
    _mainWindow->show();
}

void QuteApplication::run()
{
    _qapp.exec();
}

