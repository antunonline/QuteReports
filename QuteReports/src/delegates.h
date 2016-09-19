#ifndef DELEGATES_H
#define DELEGATES_H
#include <functional>
#include "entities.h"
#include <QWidget>

typedef std::function<Report ()> ReportSourceDelegate;
typedef std::function<void (const Report &)> ReportDestinationDelegate;
typedef std::function<Report (const int id)> ReportLoaderDelegate;
typedef std::function<QWidget*(const Report &)> ReportWidgetBuilderDelegate;
typedef std::function<QWidget*(const int )> ReportIdWidgetBuilderDelegate;


#endif // DELEGATES_H
