TARGET = nemofolderlistmodel
PLUGIN_IMPORT_PATH = org/nemomobile/folderlistmodel

TEMPLATE = lib
CONFIG += qt plugin hide_symbols
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += qml
    target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
    qmldir.path +=  $$[QT_INSTALL_QML]/$$$$PLUGIN_IMPORT_PATH
} else {
    QT += declarative
    target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
    qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$$$PLUGIN_IMPORT_PATH
}

qmldir.files += $$_PRO_FILE_PWD_/qmldir

INSTALLS += target
INSTALLS += qmldir

SOURCES += plugin.cpp \
           dirmodel.cpp \
           iorequest.cpp \
           iorequestworker.cpp \
           ioworkerthread.cpp

HEADERS += plugin.h \
           dirmodel.h \
           iorequest.h \
           iorequestworker.h \
           ioworkerthread.h

