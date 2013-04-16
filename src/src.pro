TARGET = nemofolderlistmodel
PLUGIN_IMPORT_PATH = org/nemomobile/folderlistmodel

TEMPLATE = lib
CONFIG += qt plugin hide_symbols
QT += declarative

target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$$$PLUGIN_IMPORT_PATH
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

