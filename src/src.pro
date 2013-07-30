TARGET = nemofolderlistmodel
PLUGIN_IMPORT_PATH = org/nemomobile/folderlistmodel
PLUGIN_URI         = org.nemomobile.folderlistmodel

# plugin.h and plugin.cpp use URI from PLUGIN_URI instead of hard coded
DEFINES += PLUGIN_URI=$$PLUGIN_URI

# Input
SOURCES += plugin.cpp
HEADERS += plugin.h

#core:  sources + headers, separated here to use in regression test project
include (folderlistmodel.pri)

## QApplication::clipboard() needs gui
QT += gui


TEMPLATE = lib
CONFIG += qt plugin hide_symbols
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += quick widgets
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
