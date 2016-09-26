#include "reporttype1editor.h"
#include "ui_reporttype1editor.h"
#include <QMainWindow>


#include <QDebug>
#include <QAbstractItemModel>
#include <QVector>
#include "src/entities.h"
#include "src/widget/reporttype1.h"
#include <algorithm>
#include <tuple>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <functional>

typedef std::function<QString(bool, QString const&)> StringReadWriteDelegate;

class ReportType1Controller : public QAbstractItemModel {
    QString _query;
    QVector<ReportType1DynamicInput> _inputs;

public:
    ReportType1Controller(){

    }

    ReportType1Controller(const QString & data){
        deserializeJsonState(data);
    }

    void addDynamicInput(const QModelIndex &mi,  QString const & type, QString const & name, QString const & label, const QVariant & other = QVariant::Invalid) {
        beginInsertRows(QModelIndex(), _inputs.size(), _inputs.size());
        _inputs.push_back({type, name, label, other});
        endInsertRows();
    }

    auto findDynamicInput(QString const & name) -> decltype(_inputs.begin()){
        auto found = std::find_if(_inputs.begin(), _inputs.end(), [&name](decltype(*_inputs.begin()) const &e) -> bool{
            return e.name() == name;
        });
        return found;
    }

    void removeDynamicInput(const QModelIndex &mi) {
        if(mi.isValid() && mi.row() < _inputs.size()) {
            beginRemoveRows(QModelIndex(), mi.row(), mi.row());
            _inputs.remove(mi.row());
            endRemoveRows();
        }

    }

    void updateDynamicInput(const QModelIndex &mi, QString const & name, QString const & type, const QString & label, const QVariant & other = QVariant::Invalid) {
        if(!mi.isValid())
            return;

        _inputs[mi.row()] = {type, name, label, other};
        dataChanged(createIndex(mi.row(),0), createIndex(mi.row(), 2));
        dataChanged(createIndex(0,0), createIndex(rowCount(), columnCount()));
    }

    const ReportType1DynamicInput & getDynamicInput(const int row) const{
        return _inputs[row];
    }

    void setQuery(const QString & query){
        _query = query;
    }

    const QString & query() const {
        return _query;
    }

    QString serializeReportDataToJson(QString const & query, const QVector<ReportType1DynamicInput> inputs){
        QJsonDocument doc;
        QJsonObject rootObj;

        rootObj["query"] = {query};

        QJsonArray jsonInputsArray;
        for(const ReportType1DynamicInput &i : inputs){
            QJsonArray inputArray;
            inputArray.push_back(ReportType1DynamicInput::inputTypeToString(i.type()));
            inputArray.push_back(i.name());
            inputArray.push_back(i.label());
            if(i.type() == ReportType1DynamicInput::InputType::SQLCOMBOBOX || i.type() == ReportType1DynamicInput::InputType::SUBQUERY){
                inputArray.push_back(i.variant().toString());
            }

            jsonInputsArray.push_back(inputArray);
        }

        rootObj["inputElements"] = jsonInputsArray;
        doc.setObject(rootObj);
        return doc.toJson();
    }

    QString serializeToJson(const QString & query){
        return serializeReportDataToJson(query, _inputs);
    }

    void deserializeJsonState(const QString data){
        beginRemoveRows({}, 0, _inputs.size());
        _inputs.clear();;
        endRemoveRows();

        auto unserialized = unserializeReportType1Json(data);
        _query = std::get<0>(unserialized);
        _inputs = std::get<1>(unserialized);

        beginInsertRows({}, 0, _inputs.size());
        endInsertRows();
    }

// QAbstractItemModel implementation for dynamic input fields

    QModelIndex index(int row, int column, const QModelIndex &parent) const {
        (void) parent;
        if(row > _inputs.size() || column > 3)
            return QModelIndex();
        else
            return this->createIndex(row, column, nullptr);
    }
    QModelIndex parent(const QModelIndex &child) const {
        (void)child;
        return QModelIndex();
    }
    int rowCount(const QModelIndex &parent= QModelIndex()) const{
        (void)parent;
        return _inputs.size();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const{
        (void)parent;
        return 3;
    }
    QVariant data(const QModelIndex &index, int role) const{
        if(role == Qt::DisplayRole && index.row() < _inputs.size()){
            auto input = _inputs[index.row()];
            switch (index.column()) {
            case 0:
                return input.name();
                break;
            case 1:
                return input.label();
                break;
            case 2:
                switch(input.type()){
                case ReportType1DynamicInput::InputType::TEXT:
                    return QVariant("TEXT");
                break;
                case ReportType1DynamicInput::InputType::DATE:
                    return QVariant("DATE");
                break;
                case ReportType1DynamicInput::InputType::DATETIME:
                    return QVariant("DATETIME");
                break;
                case ReportType1DynamicInput::InputType::COMBOBOX:
                    return QVariant("COMBOBOX");
                break;
                case ReportType1DynamicInput::InputType::SQLCOMBOBOX:
                    return QVariant("SQLCOMBOBOX");
                break;
                case ReportType1DynamicInput::InputType::SUBQUERY:
                    return QVariant("SUBQUERY");
                break;
                case ReportType1DynamicInput::InputType::NONE:
                    return QVariant("NONE");
                break;
                case ReportType1DynamicInput::InputType::TIME:
                    return QVariant("NONE");
                break;
                }

                break;
            default:
                break;
            }
        } else {
            return QVariant(QVariant::Invalid);
        }
        return QVariant(QVariant::Invalid);
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const{
        (void) orientation;
        if(role == Qt::DisplayRole) {
            switch(section){
            case 0:
                return "Naziv";
                break;
            case 1:
                return "Labela";
                break;
            case 2:
                return "Tip";
                break;
            default:
                return QVariant::Invalid;
                break;
            }
        }

        return QVariant::Invalid;
    }
};

ReportType1Editor::ReportType1Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportType1Editor)
{
    ui->setupUi(this);
    ui->sqlComboboxText->hide();
    _controller = new ReportType1Controller();
    connectEvents();
    populateFields();
}

ReportType1Editor::ReportType1Editor(const Report & report, QWidget *parent) :  ReportType1Editor(parent)
{
    _report = report;
    _controller = new ReportType1Controller(report.data());
    populateFields();
}


ReportType1Editor::~ReportType1Editor()
{
    delete ui;
    if(_controller){
        delete _controller;
        _controller = nullptr;
    }
}

void ReportType1Editor::setReport(const Report &report)
{
    _report = report;
    populateFields();
}

void ReportType1Editor::setupTable()
{
    ui->inputElementsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->inputElementsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->inputElementsTable->setModel(_controller);
}

void ReportType1Editor::connectEvents()
{
    connect(ui->inputElementsTable, &QTableView::clicked, [this](const QModelIndex &mi){
        int row = mi.row();
        auto i = _controller->getDynamicInput(row);
        ui->inputLabel->setText(i.label());
        ui->inputName->setText(i.name());
        ui->inputType->setCurrentText(i.typeAsString());
        if(i.type() == ReportType1DynamicInput::InputType::COMBOBOX || i.type() == ReportType1DynamicInput::InputType::SUBQUERY)
            ui->sqlComboboxText->setPlainText(i.variant().toString());
        else
            ui->sqlComboboxText->setPlainText("");

    });
    connect(ui->addInputBtn, &QPushButton::clicked, [this](){
        QVariant other;
        if(ui->inputType->currentText() == "SQLCOMBOBOX")
            other = ui->sqlComboboxText->toPlainText();
        else if(ui->inputType->currentText() == "SUBQUERY")
            other = ui->sqlComboboxText->toPlainText();
        _controller->addDynamicInput({}, ui->inputType->currentText(), ui->inputName->text(), ui->inputLabel->text(), other);
        clearInputElementFields();
        qDebug() << "Add input";
    });

    connect(ui->editInputBtn, &QPushButton::clicked, [this](){
        if(ui->inputElementsTable->selectionModel()->selectedRows().size() == 0)
            return;
        QVariant other;
        if(ui->inputType->currentText() == "SQLCOMBOBOX")
            other = ui->sqlComboboxText->toPlainText();
        else if(ui->inputType->currentText() == "SUBQUERY")
            other = ui->sqlComboboxText->toPlainText();
        auto im = ui->inputElementsTable->selectionModel()->selectedRows().first();
        _controller->updateDynamicInput(im,  ui->inputName->text(), ui->inputType->currentText(), ui->inputLabel->text(), other);
        qDebug() << "Update input";
    });

    connect(ui->deleteInputBtn, &QPushButton::clicked, [this](){
        if(ui->inputElementsTable->selectionModel()->selectedRows().size() == 0)
            return;
        auto im = ui->inputElementsTable->selectionModel()->selectedRows().first();
        _controller->removeDynamicInput(im);
        qDebug() << "Remove input";
    });

    connect(ui->clearInputBtn, &QPushButton::clicked, [this](){
        clearInputElementFields();
        qDebug() << "Clear input";
    });

    connect(ui->saveReportBtn, &QPushButton::clicked, [this](){
        saveReport(constructReport());
        qDebug() << "Construct report";
    });

    connect(ui->cancelReportBtn, &QPushButton::clicked, [this](){
        QMainWindow *w = dynamic_cast<QMainWindow*>(this->parent());
        if(w){
            w->close();
            w->deleteLater();
        }
    });

    connect(ui->showReportBtn, &QPushButton::clicked, [this](){
        showReport(constructReport());
    });

    connect(ui->comboboxSql, &QPushButton::clicked, [this](){
        if(ui->comboboxSql->isChecked())
            ui->sqlComboboxText->show();
        else
            ui->sqlComboboxText->hide();
    });
}

void ReportType1Editor::clearInputElementFields()
{
    ui->inputName->setText("");
    ui->inputLabel->setText("");
    ui->sqlComboboxText->setPlainText("");
}

void ReportType1Editor::populateFields()
{
    _controller->deserializeJsonState(_report.data());
    ui->reportName->setText(_report.name());
    ui->reportDescription->setText(_report.description());
    ui->sqlComboboxText->setPlainText("");
    ui->sqlEditor->setPlainText(_controller->query());
    setupTable();
}

void ReportType1Editor::showReport(const Report &report)
{
    ui->reportWidget->deleteLater();
    ui->reportWidget = new ReportType1(report, this);
    ui->reportWidgetHolder->addWidget(ui->reportWidget);
}

Report ReportType1Editor::constructReport()
{
    auto data = _controller->serializeToJson(ui->sqlEditor->toPlainText());
    QString name = ui->reportName->text(), description = ui->reportDescription->text();
    return {_report.id(), name, description, "ReportType1",  data};
}
