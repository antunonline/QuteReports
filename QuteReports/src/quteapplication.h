#ifndef QUTEAPPLICATION_H
#define QUTEAPPLICATION_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QSettings>
#include <QString>
#include "qutemainwindow.h"
#include <memory>
#include "src/reportmodel.h"
#include "reporteditorcontroller.h"
#include "reportviewcontroller.h"

class QuteApplication
{
//    QGuiApplication _guiApp;
    QApplication _qapp;
     QQmlApplicationEngine _engine;
    QSettings * _settings;
    std::unique_ptr<QuteMainWindow> _mainWindow;
    ReportModel _reportModel;
    ReportEditController _reportEditorController;
    ReportViewController _reportViewController;


public:
    QuteApplication(int argc, char *argv[]);


    static void registerCustomQmlTypes();

    void bootstrap();
    void bootstrapWindow();


    void run();
};

#endif // QUTEAPPLICATION_H
