#ifndef QMLITEMMODELXLSXEXPORTER_H
#define QMLITEMMODELXLSXEXPORTER_H
#include "qobject.h"
#include "qstring.h"
#include "QFile"
#include <QAbstractItemModel>

class QmlItemModelXlsxExporter : public QObject
{
    Q_OBJECT
public:
    QmlItemModelXlsxExporter();

    Q_INVOKABLE void exportQmlModel(QString const & fileLocation, QAbstractItemModel * im) const;
    Q_INVOKABLE void exportModel(QString const & filename, const QAbstractItemModel & im) const;
};

#endif // QMLITEMMODELXLSXEXPORTER_H
