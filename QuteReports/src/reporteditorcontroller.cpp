#include "reporteditorcontroller.h"
#include "widget/reporttype1editor.h"
#include <QMainWindow>

ReportEditController::ReportEditController(ReportModel &reportModel) : _reportModel{reportModel}
{

}

void ReportEditController::showReport1Editor()
{
    showreport1Editor({});
}

void ReportEditController::showreport1Editor(const Report &r)
{
    QMainWindow * reportWindow = new QMainWindow();
    reportWindow->setMinimumSize(640, 480);
    ReportType1Editor *reportEditor = new ReportType1Editor(r, reportWindow);
    QMainWindow::connect(reportEditor, &ReportType1Editor::saveReport, [this, reportEditor](const Report &r){
        if(r.id() > 0){
            _reportModel.update(r);
        }else {
            auto newR = r.setId(_reportModel.insert(r));
            reportEditor->setReport(newR);
        }
    });
    reportWindow->setCentralWidget(reportEditor);
    reportWindow->show();
    (void) reportEditor;
}

void ReportEditController::editSelected()
{

}
