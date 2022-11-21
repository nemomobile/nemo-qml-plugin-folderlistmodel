TARGET = nemofolderlistmodel
PLUGIN_IMPORT_PATH = org/nemomobile/folderlistmodel

TEMPLATE = lib
QT = core qml gui quick widgets
CONFIG += qt plugin hide_symbols

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
qmldir.path +=  $$[QT_INSTALL_QML]/$$$$PLUGIN_IMPORT_PATH

qmldir.files += $$_PRO_FILE_PWD_/qmldir

INSTALLS += target
INSTALLS += qmldir

INCLUDEPATH  += trash disk smb net

include (smb/qsambaclient/qsambaclient.pri)

SOURCES += cleanurl.cpp \
           clipboard.cpp \
           diriteminfo.cpp \
           dirmodel.cpp \
           dirselection.cpp \
           externalfswatcher.cpp \
           filecompare.cpp \
           filesystemaction.cpp \
           fmutil.cpp \
           imageprovider.cpp \
           iorequest.cpp \
           iorequestworker.cpp \
           ioworkerthread.cpp \
           location.cpp \
           locationitemdir.cpp \
           locationitemdiriterator.cpp \
           locationitemfile.cpp \
           locationsfactory.cpp \
           locationurl.cpp \
           networklistworker.cpp \
           networklocation.cpp \
           plugin.cpp \
           urliteminfo.cpp


HEADERS += cleanurl.h \
           clipboard.h \
           diriteminfo.h \
           dirmodel.h \
           dirselection.h \
           externalfswatcher.h \
           filecompare.h \
           filesystemaction.h \
           fmutil.h \
           imageprovider.h \
           iorequest.h \
           iorequestworker.h \
           ioworkerthread.h \
           location.h \
           locationitemdir.h \
           locationitemdiriterator.h \
           locationitemfile.h \
           locationsfactory.h \
           locationurl.h \
           networklistworker.h \
           networklocation.h \
           plugin.h \
           urliteminfo.h

