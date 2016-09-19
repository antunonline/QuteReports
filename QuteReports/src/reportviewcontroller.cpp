#include "reportviewcontroller.h"
#include "widget/reporttype1.h"

ReportViewController::ReportViewController(ReportModel &model) : _reportModel{model}
{

}

QWidget *ReportViewController::createReportWidget(const Report &r)
{
    if(r.type() == "ReportType1"){
         ReportType1 *report = new ReportType1(r);
         report->setObjectName(r.name());
         return report;
    }
    return nullptr;
}
