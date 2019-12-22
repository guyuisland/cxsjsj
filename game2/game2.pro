#-------------------------------------------------
#
# Project created by QtCreator 2019-10-23T08:04:44
#
#-------------------------------------------------

QT       += core gui
QT += concurrent
QT += network
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lpthread libwsock32 libws2_32
TARGET = game2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    register.cpp \
    gamelobby.cpp \
    fightui.cpp \
    mylabel.cpp \
    clientsocket.cpp \
    stackwidget.cpp \
    rankingroom.cpp \
    waitingroom.cpp \
    monster.cpp \
    skill.cpp \
    player.cpp \
    monsterfactory.cpp \
    fightenv.cpp

HEADERS += \
        mainwindow.h \
    register.h \
    gamelobby.h \
    fightui.h \
    mylabel.h \
    clientsocket.h \
    json.hpp \
    socket.h \
    protocol.h \
    stackwidget.h \
    rankingroom.h \
    waitingroom.h \
    monster.h \
    player.h \
    skill.h \
    monsterfactory.h \
    fightenv.h

FORMS += \
        mainwindow.ui \
    register.ui \
    gamelobby.ui \
    fightui.ui \
    stackwidget.ui \
    rankingroom.ui \
    waitingroom.ui \
    fightenv.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    res.qrc
