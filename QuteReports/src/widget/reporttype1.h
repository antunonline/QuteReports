#ifndef REPORTTYPE1_H
#define REPORTTYPE1_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>
#include "src/entities.h"
#include "src/components/queryexectaskrunner.h"
#include <QPushButton>
#include <QVariant>
#include <QMap>

namespace Ui {
class ReportType1;
}

class ReportType1DynamicInput {
public:
    enum class InputType {
        NONE,
        TEXT,
        DATE,
        TIME,
        DATETIME,
        COMBOBOX,
        SQLCOMBOBOX,
        SUBQUERY
    };
private:
    InputType _type;
    QString _name, _label, _comboboxSql;
    QVariant _variant;

public:
    ReportType1DynamicInput();
    ReportType1DynamicInput(const QString & type, QString const & name, QString const & label, QVariant const & other = QVariant::Invalid);
    ReportType1DynamicInput(const InputType &type, QString const & name, QString const & label, QVariant const & other = QVariant::Invalid);

    InputType type() const;
    QString typeAsString() const;
    const QString & name() const;
    const QString & label() const;
    static InputType toInputType(const QString & type);
    QVariant variant() const;
    static QString inputTypeToString(const InputType &it);
};


std::tuple<QString, QVector<ReportType1DynamicInput>> unserializeReportType1Json(const QString &report);

class ReportType1 : public QWidget
{
    Q_OBJECT

public:
    explicit ReportType1(QWidget *parent = 0);
    explicit ReportType1(QString const & query, QVector<ReportType1DynamicInput> const &dynamicInput,  QWidget *parent = 0);
    explicit ReportType1(const Report &report,  QWidget *parent = 0);
    ~ReportType1();
    typedef ReportType1DynamicInput::InputType TYPE;

private:
    Ui::ReportType1 *ui;
    QString _query;
    QVector<ReportType1DynamicInput> _dynamicInput;
    QueryExecTaskRunner *_queryTaskRunner = nullptr;
    // This model ptr is used in QTreeView, so remove it from it reaching refcount 0 (assigning new shared pointer to it). Otherwise it will seqfault
    std::shared_ptr<BufferedQueryModel> _queryModel;
    QPushButton * _runReportBtn =nullptr;
private slots:
    void runReport();
    void on_excelExportBtn_clicked();

protected:
    void init();
    void generateDynamicInput();
    void generateLabel(QString const & str);
    void generateText(ReportType1DynamicInput const &ti);
    void generateDate(ReportType1DynamicInput const &ti);
    void generateTime(ReportType1DynamicInput const &ti);
    void generateDateTime(ReportType1DynamicInput const &ti);
    void generateSqlCombobox(ReportType1DynamicInput const &ti);
    void generateSqlSubquery(ReportType1DynamicInput const &ti);
    void generateLastElements();
};

#endif // REPORTTYPE1_H
