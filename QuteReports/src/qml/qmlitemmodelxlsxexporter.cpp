#include "qmlitemmodelxlsxexporter.h"
#include "xlsxdocument.h"
#include "QDebug"
#include "QUrl"
#include "QFileDialog"

QmlItemModelXlsxExporter::QmlItemModelXlsxExporter()
{

}

void QmlItemModelXlsxExporter::exportQmlModel(const QString &fileLocation, QAbstractItemModel * im) const
{
    QFile f{ QUrl(fileLocation).toLocalFile()};
    f.open(QFile::WriteOnly | QFile::Truncate);


    QXlsx::Document d;
    const int rowCount = im->rowCount();
    const auto roles = im->roleNames().keys();

    for(int i = 0, j = 1; i < rowCount; i++, j = 1)
        for(auto & v : roles)
            d.write(i+1, j++, im->data(im->index(i,0), v));

    d.saveAs(&f);

    f.close();

}

void QmlItemModelXlsxExporter::exportModel(const QString &filename, const QAbstractItemModel &im) const
{
    QXlsx::Document d;
    const int rowCount = im.rowCount(),
            columnCount = im.columnCount();

    for(int i = 0, j = 1; i < rowCount; i++, j = 1)
        for(int j = 0; j < columnCount; j++)
            d.write(i+1, j+1, im.data(im.index(i,j)));

    d.saveAs(filename);
}

