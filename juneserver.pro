QT += gui core websockets widgets


CONFIG += c++14
if(debug){
    CONFIG += console
}
CONFIG -= app_bundle
TEMPLATE = app

release: TARGET = juneserver
debug: TARGET = juneserverd

DESTDIR = $$(DEV_BIN)

release: BUILDDIR = build/release
debug:   BUILDDIR = build/debug

OBJECTS_DIR = $$BUILDDIR/.obj
MOC_DIR = $$BUILDDIR/.moc
RCC_DIR = $$BUILDDIR/.qrc
UI_DIR = $$BUILDDIR/.ui


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$(JUNE_ROOT)

if(debug){

    LIBS += -L$$(DEV_LIBS) -ltagsystemd -linfluxdbd -lpluginLoadd
#LIBS += -ltagsystemd
}
else{
    LIBS += -L$$(DEV_LIBS) -ltagsystem -linfluxdb -lpluginLoad
}





SOURCES += main.cpp \
    addlogvaluedialog.cpp \
    app.cpp \
    logvaluedata.cpp \
    logvaluemodel.cpp \
    logvalueview.cpp \
    pluginmanager.cpp \
    requests/loadpluginlistener.cpp \
    rest/connection.cpp \
    rest/connectionmanager.cpp \
    rest/httprequest.cpp \
    rest/requestlistener.cpp \
    rest/respons.cpp \
    rest/restserver.cpp \
    websocketserver.cpp \
    mainwindow.cpp \
    gui/menubar.cpp \
    gui/clientlistwidget.cpp \
    client.cpp \
    clientinformation.cpp \
    logger.cpp \
    gui/loggerwidget.cpp

HEADERS += \
    addlogvaluedialog.h \
    app.h \
    logvaluedata.h \
    logvaluemodel.h \
    logvalueview.h \
    pluginmanager.h \
    requests/loadpluginlistener.h \
    rest/connection.h \
    rest/connectionmanager.h \
    rest/httprequest.h \
    rest/requestlistener.h \
    rest/respons.h \
    rest/restserver.h \
    websocketserver.h \
    mainwindow.h \
    gui/menubar.h \
    gui/clientlistwidget.h \
    client.h \
    clientinformation.h \
    logger.h \
    gui/loggerwidget.h

FORMS += \
    addlogvaluedialog.ui \
    logvalueview.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc
