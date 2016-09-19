#include "qutemainwindow.h"
#include "ui_qutemainwindow.h"
#include <QMainWindow>
#include <QBoxLayout>
#include <utility>

QuteMainWindow::QuteMainWindow(ReportEditController &rec, QWidget *parent) :
    QMainWindow(parent),
    _reportList(this),
    ui(new Ui::QuteMainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(&_reportList);
    _reportList.setWidgetBuilderDelegate([this](const int reportId){
        Report report = _reportLoader(reportId);
        QWidget * widget = _widgetBuilderDelegate(report);
        return widget;
    });
}

QuteMainWindow::~QuteMainWindow()
{
    delete ui;
}

void QuteMainWindow::setReportEditorDelegate(ReportDestinationDelegate editor)
{
    _editorDelegate = editor;
}

void QuteMainWindow::setReportWidgetBuilderDelegate(ReportWidgetBuilderDelegate widgetBuilder)
{
    _widgetBuilderDelegate = widgetBuilder;
}

void QuteMainWindow::setReportLoaderDelegate(ReportLoaderDelegate loaderDelegate)
{
    _reportLoader = loaderDelegate;
}

void QuteMainWindow::setDeleteReportDelegate(ReportDestinationDelegate deleteReport)
{
    _deleteReport = deleteReport;
}

void QuteMainWindow::on_actionNew_Report_v1_triggered()
{
    _editorDelegate({});
}

void QuteMainWindow::on_actionEdit_Report_v1_triggered()
{
    const auto reportId = _reportList.selectedReportId();
    if(reportId ==0 ) return;

    const auto report = _reportLoader(reportId);
    _editorDelegate(report);
}

void QuteMainWindow::connectEvents()
{

}

void QuteMainWindow::on_actionDelete_Report_triggered()
{
    const auto reportId = _reportList.selectedReportId();
    if(reportId ==0 ) return;

    const auto report = _reportLoader(reportId);
    _deleteReport(report);
}
