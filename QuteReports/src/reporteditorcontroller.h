#ifndef REPORTEDITORCONTROLLER_H
#define REPORTEDITORCONTROLLER_H
#include "src/entities.h"
#include "reportmodel.h"
#include <functional>

class ReportEditController
{
    ReportModel & _reportModel;
public:
    ReportEditController(ReportModel & reportModel);
    void showReport1Editor();
    void showreport1Editor(const Report &r);
    void editSelected();
};

#endif // REPORTEDITORCONTROLLER_H
