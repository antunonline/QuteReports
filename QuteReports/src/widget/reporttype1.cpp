#include "reporttype1.h"
#include "ui_reporttype1.h"
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QDebug>
#include <QDateTimeEdit>
#include <QSqlError>
#include <tuple>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include "src/qml/qmlitemmodelxlsxexporter.h"
#include <QComboBox>

std::tuple<QString, QVector<ReportType1DynamicInput>> unserializeReportType1Json(const QString&data){
    // { query: query, inputElements: [ dynamicInput  ]  }
    // dynamicInput = { type, name, label }
    QString _query;
    QVector<ReportType1DynamicInput> dynamicElements;

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject obj = doc.object();
    auto const end = obj.end();

    if(obj.find("query") != end){
        _query = obj["query"].toString();
    }

    if(obj.find("inputElements") != end) {
        QJsonArray inputElements = obj["inputElements"].toArray();
        for(QJsonValue v :inputElements){
            if(v.isArray()){
                auto inputElement = v.toArray();
                if(inputElement.size() >= 3){
                    QString eType, eName, eLabel;
                    eType = inputElement[0].toString();
                    eName = inputElement[1].toString();
                    eLabel = inputElement[2].toString();
                    QVariant other;
                    if(ReportType1DynamicInput::toInputType(eType) == ReportType1DynamicInput::InputType::SQLCOMBOBOX &&
                            inputElement.size()>3)
                        other = inputElement[3].toString();
                    else if(ReportType1DynamicInput::toInputType(eType) == ReportType1DynamicInput::InputType::SUBQUERY &&
                            inputElement.size()>3)
                        other = inputElement[3].toString();


                    dynamicElements.push_back({eType, eName, eLabel, other});
                }
            }
        }
    }

    return std::make_tuple(_query, dynamicElements);
}


ReportType1::ReportType1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportType1) {
    ui->setupUi(this);
    _queryModel = new BufferedQueryModel();
}

ReportType1::ReportType1(QString const & report, QVector<ReportType1DynamicInput> const &dynamicInput,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportType1),
    _query{report},
    _dynamicInput{dynamicInput}
{
    _queryModel = new BufferedQueryModel();
    ui->setupUi(this);
    generateDynamicInput();
    _queryModel->setQuery(_query);
    connect(_queryModel, &BufferedQueryModel::dataLoaded, [this](){
        this->_runReportBtn->setDisabled(false);
    });
}

ReportType1::ReportType1(const Report &report, QWidget *parent) :
    ui(new Ui::ReportType1)
{
    (void)parent;
    _queryModel = new BufferedQueryModel();
    ui->setupUi(this);
    auto tpl = unserializeReportType1Json(report.data());
    _query = std::get<0>(tpl);

    _queryModel->setQuery(_query);
    connect(_queryModel, &BufferedQueryModel::dataLoaded, [this](){
        this->_runReportBtn->setDisabled(false);
    });

    _dynamicInput = std::get<1>(tpl);
    generateDynamicInput();
}

ReportType1::~ReportType1()
{
    _queryModel->deleteLater();
    delete ui;
}

void ReportType1::runReport()
{
    this->_runReportBtn->setDisabled(true);
    qDebug() << "Execute";
    _queryModel->setQuery(_query);


    for(const ReportType1DynamicInput &i : _dynamicInput){
        switch(i.type()){
            case TYPE::TEXT:
            _queryModel->bindValue(":"+i.name(), findChild<QLineEdit*>(i.name())->text());
            break;
            case TYPE::DATE:
            _queryModel->bindValue(":"+i.name(), findChild<QDateTimeEdit*>(i.name())->date().toString("yyyy-MM-dd"));
            qDebug() << i.name() << "DATE" << findChild<QDateTimeEdit*>(i.name())->date().toString("yyyy-MM-dd");
            break;
            case TYPE::DATETIME:
            _queryModel->bindValue(":"+i.name(), findChild<QDateTimeEdit*>(i.name())->dateTime().toString("yyyy-MM-dd HH:mm"));
            qDebug() << i.name() << "DATETIME: " << findChild<QDateTimeEdit*>(i.name())->dateTime().toString("yyyy-MM-dd HH:mm");
            break;
        case TYPE::COMBOBOX:
            break;
        case TYPE::SQLCOMBOBOX:
            _queryModel->bindValue(":"+i.name(), findChild<QComboBox*>(i.name())->currentText());
            qDebug() << i.name() << "SQLCOMBOBOX: " << findChild<QComboBox*>(i.name())->currentText();
            break;
        case TYPE::SUBQUERY:
            qDebug() << i.name() << "SUBQUERY: " << findChild<QComboBox*>(i.name())->currentText();
            _queryModel->bindSubquery(":"+i.name(), property(i.name()+"_query").toString(), findChild<QLineEdit*>(i.name())->text());
            break;
        case TYPE::TIME:
            break;
        case TYPE::NONE:
            break;

        }
    }

    _queryModel->exec();
    ui->treeView->setModel(_queryModel);

}

void ReportType1::generateDynamicInput()
{
    for(const ReportType1DynamicInput &i : _dynamicInput){
        switch(i.type()){
        case TYPE::NONE:
            break;
        case TYPE::TEXT:
            generateText(i);
            break;
        case TYPE::DATE:
            generateDate(i);
            break;
        case TYPE::TIME:
            generateDateTime(i);
            break;
        case TYPE::DATETIME:
            generateDateTime(i);
            break;
        case TYPE::COMBOBOX:
            break;
        case TYPE::SQLCOMBOBOX:
            generateSqlCombobox(i);
            break;
        case TYPE::SUBQUERY:
            generateSubquery(i);
            break;
        }
    }
    generateLastElements();
}

void ReportType1::generateLabel(const QString &txt)
{

    QLabel *label = new QLabel(this);
    label->setText(txt);
    ui->controlsLayout->addWidget(label);
}

void ReportType1::generateText(const ReportType1DynamicInput &ti)
{
    generateLabel(ti.label());
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName(ti.name());
    ui->controlsLayout->addWidget(edit);
}

void ReportType1::generateDate(const ReportType1DynamicInput &ti)
{
    generateLabel(ti.label());
    QDateTimeEdit * date = new QDateTimeEdit(this);
    date->setDisplayFormat("dd.MM.yyyy.");
    date->setDate(QDate::currentDate());
    date->setCalendarPopup(true);
    date->setObjectName(ti.name());

    ui->controlsLayout->addWidget(date);
}

void ReportType1::generateTime(const ReportType1DynamicInput &ti)
{
    generateLabel(ti.label());
    QDateTimeEdit *time = new QDateTimeEdit(this);
    time->setObjectName(ti.name());
    ui->controlsLayout->addWidget(time);
}

void ReportType1::generateDateTime(const ReportType1DynamicInput &ti)
{
    generateLabel(ti.label());
    QDateTimeEdit *datetime = new QDateTimeEdit(this);
    datetime->setDisplayFormat("HH:mm dd.MM.yyyy");
    datetime->setCalendarPopup(true);
    datetime->setDate(QDate::currentDate());
    datetime->setObjectName(ti.name());
    ui->controlsLayout->addWidget(datetime);
}

void ReportType1::generateSqlCombobox(const ReportType1DynamicInput &ti)
{
    auto sql = ti.comboboxSql();
    QSqlQueryModel * model = new QSqlQueryModel(this);
    model->setQuery(sql);
    QComboBox *box = new QComboBox(this);
    box->setObjectName(ti.name());
    box->setModel(model);

    generateLabel(ti.label());
    ui->controlsLayout->addWidget(box);
}

void ReportType1::generateSubquery(const ReportType1DynamicInput &ti)
{
    generateLabel(ti.label());
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName(ti.name());
    ui->controlsLayout->addWidget(edit);
    // _inputElementData[ti.name()] =
}

void ReportType1::generateLastElements()
{
    _runReportBtn = new QPushButton(this);

    _runReportBtn->setText("Prikaži");
    connect(_runReportBtn, &QPushButton::clicked, this, &ReportType1::runReport);
    ui->controlsLayout->addWidget(_runReportBtn);
    ui->controlsLayout->addStretch();
}

ReportType1DynamicInput::ReportType1DynamicInput() : _type{InputType::NONE}
{

}

ReportType1DynamicInput::ReportType1DynamicInput(const QString &type, QString const & name, QString const & label, QVariant const & other) :
        _type{toInputType(type)}, _name{name}, _label{label}
{
    if(_type == InputType::SQLCOMBOBOX){
        _comboboxSql = other.toString();
    }
}

ReportType1DynamicInput::ReportType1DynamicInput(const ReportType1DynamicInput::InputType &type, QString const & name, QString const & label, QVariant const & other) :
        _type{type}, _name{name}, _label{label}
{
    if(type == InputType::SQLCOMBOBOX){
        _comboboxSql = other.toString();
    }
}

ReportType1DynamicInput::InputType ReportType1DynamicInput::type() const
{
    return _type;
}

QString ReportType1DynamicInput::typeAsString() const
{
    return inputTypeToString(_type);
}

const QString &ReportType1DynamicInput::name() const
{
    return _name;
}

const QString &ReportType1DynamicInput::label() const
{
    return _label;
}

const QString &ReportType1DynamicInput::comboboxSql() const
{
    return _comboboxSql;
}

QVariant ReportType1DynamicInput::other() const
{
    return _other;
}

ReportType1DynamicInput::InputType ReportType1DynamicInput::toInputType(const QString &type)
{
    const auto t = type.toLower();
    if(t == "none")
        return InputType::NONE;
    else if(t=="text")
        return InputType::TEXT;
    else if(t=="date")
        return InputType::DATE;
    else if(t=="time")
        return InputType::TIME;
    else if(t=="datetime")
        return InputType::DATETIME;
    else if(t=="combobox")
        return InputType::COMBOBOX;
    else if(t=="sqlcombobox")
        return InputType::SQLCOMBOBOX;
    else
        return InputType::NONE;
}

QString ReportType1DynamicInput::inputTypeToString(const ReportType1DynamicInput::InputType &it)
{
    switch(it){
    case InputType::COMBOBOX:
        return "COMBOBOX";
        break;
    case InputType::SQLCOMBOBOX:
        return "SQLCOMBOBOX";
        break;
    case InputType::DATE:
        return "DATE";
        break;
    case InputType::DATETIME:
        return "DATETIME";
        break;
    case InputType::TEXT:
        return "TEXT";
        break;
    case InputType::TIME:
        return "TIME";
        break;
    default:
        return "NONE";
        break;
    }
}


void ReportType1::on_excelExportBtn_clicked()
{
    auto fileName = QFileDialog::getSaveFileName(this);
    if(fileName.size() == 0 )return;
    QmlItemModelXlsxExporter exporter;
    exporter.exportModel(fileName, *_queryModel);

}
