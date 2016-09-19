QT += qml quick sql xlsx widgets

CONFIG += c++11

SOURCES += main.cpp \
    src/qml/qmlsqlquerymodel.cpp \
    src/quteapplication.cpp \
    src/qml/qmlitemmodelxlsxexporter.cpp \
    src/qutemainwindow.cpp \
    src/widget/reportopenerwidget.cpp \
    src/widget/reporttype1.cpp \
    src/widget/reporttype1editor.cpp \
    src/reportmodel.cpp \
    src/entities.cpp \
    src/reporteditorcontroller.cpp \
    src/reportviewcontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/qml/qmlsqlquerymodel.h \
    src/quteapplication.h \
    src/qml/qmlitemmodelxlsxexporter.h \
    src/qutemainwindow.h \
    src/widget/reportopenerwidget.h \
    src/widget/reporttype1.h \
    src/widget/reporttype1editor.h \
    src/reportmodel.h \
    src/entities.h \
    src/reporteditorcontroller.h \
    src/reportviewcontroller.h \
    src/delegates.h

FORMS += \
    src/qutemainwindow.ui \
    src/widget/reportopenerwidget.ui \
    src/widget/reporttype1.ui \
    src/widget/reporttype1editor.ui
