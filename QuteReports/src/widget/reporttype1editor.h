#ifndef REPORTTYPE1EDITOR_H
#define REPORTTYPE1EDITOR_H

#include <QWidget>
#include "src/entities.h"
#include <functional>
#include <QString>

namespace Ui {
class ReportType1Editor;
}

class ReportType1Controller;
class ReportType1Editor : public QWidget
{
    ReportType1Controller *_controller;
    Q_OBJECT
    Report _report;

public:

    explicit ReportType1Editor(QWidget *parent = 0);
    explicit ReportType1Editor(const Report & report,  QWidget *parent = 0);
    ReportType1Editor(ReportType1Editor const & e) = delete;
    ~ReportType1Editor();
    void setReport(const Report & report);

signals:
    void saveReport(const Report & report);

private:
    Ui::ReportType1Editor *ui;

protected:
    void setupTable();
    void connectEvents();
    void clearInputElementFields();
    void populateFields();
    void showReport(Report const & report);
    Report constructReport();
};

#endif // REPORTTYPE1EDITOR_H
