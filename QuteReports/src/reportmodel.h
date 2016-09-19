#ifndef REPORTMODEL_H
#define REPORTMODEL_H
#include "entities.h"

class ReportModel
{
public:
    ReportModel();
    Report fetchById(const int id);
    bool update(Report const & r);
    int insert(Report const & r);
    void remove(Report const &r);

};

#endif // REPORTMODEL_H
