#ifndef REPORTOPENERWIDGET_H
#define REPORTOPENERWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "src/delegates.h"
namespace Ui {
class ReportOpenerWidget;
}

class ReportList : public QWidget
{
    Q_OBJECT
    Ui::ReportOpenerWidget *ui;
    QSqlQuery _query;
    QSqlQueryModel  _queryModel;
    ReportIdWidgetBuilderDelegate _widgetBuilder;

public:
    explicit ReportList(QWidget *parent = 0);
    void refresh();
    ~ReportList();
    int selectedReportId() const;
    void setWidgetBuilderDelegate(ReportIdWidgetBuilderDelegate d);

protected slots:
    void openReport();
    void reportSelected();
};

#endif // REPORTOPENERWIDGET_H
