#ifndef REPORTTYPE1DYNAMICFIELDEDITOR_H
#define REPORTTYPE1DYNAMICFIELDEDITOR_H

#include <QWidget>

namespace Ui {
class ReportType1DynamicFieldEditor;
}

class ReportType1DynamicFieldEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ReportType1DynamicFieldEditor(QWidget *parent = 0);
    ~ReportType1DynamicFieldEditor();

private:
    Ui::ReportType1DynamicFieldEditor *ui;
};

#endif // REPORTTYPE1DYNAMICFIELDEDITOR_H
