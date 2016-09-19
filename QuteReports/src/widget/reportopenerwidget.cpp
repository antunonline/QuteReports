#include "reportopenerwidget.h"
#include "ui_reportopenerwidget.h"
#include <QDebug>
#include <QSqlError>
#include "reporttype1.h"

ReportList::ReportList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportOpenerWidget),
    _query(),
    _queryModel{parent}
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked, [this](){this->refresh();});
    connect(ui->reportNameFilter, &QLineEdit::textChanged, [this](){this->refresh();});
    connect(ui->reportsTable, &QTableView::clicked, this, &ReportList::reportSelected);
    connect(ui->reportsTable, &QTableView::doubleClicked, this, &ReportList::openReport);
    connect(ui->openReportBtn, &QPushButton::clicked, this, &ReportList::openReport);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [this](const int tabId){
        if(tabId == 0) return;
        ui->tabWidget->removeTab(tabId);
    });

    _query.prepare("select id, name, description from reports where name like :name");
}

void ReportList::refresh(){
    QString findName = "%"+ui->reportNameFilter->text()+"%";
    _query.bindValue(":name",  findName);
    _query.exec();
    _queryModel.setQuery(_query);
    ui->reportsTable->setModel(&_queryModel);
    qDebug() << _queryModel.lastError();
}

ReportList::~ReportList()
{
    delete ui;
}

int ReportList::selectedReportId() const
{
    auto selectionModel = ui->reportsTable->selectionModel();
    if(selectionModel && selectionModel->hasSelection()){
        auto mi =_queryModel.index(selectionModel->selectedIndexes().at(0).row(), 0);
        return _queryModel.data(mi).toInt();
    }
    return 0;
}

void ReportList::setWidgetBuilderDelegate(ReportIdWidgetBuilderDelegate d)
{
    _widgetBuilder = d;
}

void ReportList::openReport()
{
    const auto reportId = selectedReportId();
    if(reportId == 0)
        return;
    auto widget = _widgetBuilder(selectedReportId());
    if(widget){
        int tabId = ui->tabWidget->addTab(widget, widget->objectName());
    }
}

void ReportList::reportSelected()
{

}
