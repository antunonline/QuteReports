#ifndef QUTEMAINWINDOW_H
#define QUTEMAINWINDOW_H

#include <QMainWindow>
#include "widget/reportopenerwidget.h"
#include "reporteditorcontroller.h"
#include "src/reportmodel.h"
#include "delegates.h"

namespace Ui {
class QuteMainWindow;
}


class QuteMainWindow : public QMainWindow
{
    Q_OBJECT
    ReportDestinationDelegate _editorDelegate, _deleteReport;
    ReportWidgetBuilderDelegate _widgetBuilderDelegate;
    ReportLoaderDelegate _reportLoader;
    ReportList _reportList;

public:
    explicit QuteMainWindow(ReportEditController & rec, QWidget *parent = 0);
    ~QuteMainWindow();
    void setReportEditorDelegate(ReportDestinationDelegate  editor);
    void setReportWidgetBuilderDelegate( ReportWidgetBuilderDelegate widgetBuilder );
    void setReportLoaderDelegate(ReportLoaderDelegate loaderDelegate);
    void setDeleteReportDelegate(ReportDestinationDelegate deleteReport);

private slots:
    void on_actionNew_Report_v1_triggered();
    void on_actionEdit_Report_v1_triggered();

    void on_actionDelete_Report_triggered();

protected:
    void connectEvents();

private:
    Ui::QuteMainWindow *ui;
};

#endif // QUTEMAINWINDOW_H
