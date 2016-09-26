#include "reporttype1dynamicfieldeditor.h"
#include "ui_reporttype1dynamicfieldeditor.h"

ReportType1DynamicFieldEditor::ReportType1DynamicFieldEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportType1DynamicFieldEditor)
{
    ui->setupUi(this);
}

ReportType1DynamicFieldEditor::~ReportType1DynamicFieldEditor()
{
    delete ui;
}
