#ifndef REPORTTYPE1_H
#define REPORTTYPE1_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>
#include "src/entities.h"
#include "src/bufferedquerymodel.h"
#include <QPushButton>

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

public:
    ReportType1DynamicInput();
    ReportType1DynamicInput(const QString & type, QString const & name, QString const & label, QVariant const & other = QVariant::Invalid);
    ReportType1DynamicInput(const InputType &type, QString const & name, QString const & label, QVariant const & other = QVariant::Invalid);

    InputType type() const;
    QString typeAsString() const;
    const QString & name() const;
    const QString & label() const;
    const QString & comboboxSql() const;
    static InputType toInputType(const QString & type);
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
    BufferedQueryModel * _queryModel;
    QVector<ReportType1DynamicInput> _dynamicInput;
    QPushButton * _runReportBtn =nullptr;
private slots:
    void runReport();
    void on_excelExportBtn_clicked();

protected:
    void generateDynamicInput();
    void generateLabel(QString const & str);
    void generateText(ReportType1DynamicInput const &ti);
    void generateDate(ReportType1DynamicInput const &ti);
    void generateTime(ReportType1DynamicInput const &ti);
    void generateDateTime(ReportType1DynamicInput const &ti);
    void generateSqlCombobox(ReportType1DynamicInput const &ti);
    void generateLastElements();
};

#endif // REPORTTYPE1_H
