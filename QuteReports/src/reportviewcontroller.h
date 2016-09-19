#ifndef REPORTVIEWCONTROLLER_H
#define REPORTVIEWCONTROLLER_H
#include "entities.h"
#include "reportmodel.h"
#include <QWidget>

class ReportViewController
{
    ReportModel & _reportModel;
public:
    ReportViewController(ReportModel & model);
    QWidget * createReportWidget(const Report &r);
};

#endif // REPORTVIEWCONTROLLER_H
