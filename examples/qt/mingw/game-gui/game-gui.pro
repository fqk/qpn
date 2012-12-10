#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T10:48:40
#
#-------------------------------------------------
TEMPLATE = app

QT      += core gui
TARGET   = game-gui
DEFINES += QT_NO_STATEMACHINE

INCLUDEPATH += . \
    $(QPN)/ports/qt \
    $(QPN)/include

HEADERS += \
    $(QPN)/ports/qt/qpn_app.h \
    $(QPN)/ports/qt/pixellabel.h \
    qpn_port.h \
    gui.h \
    bsp.h \
    game.h

SOURCES += \
    $(QPN)/source/qepn.c \
    $(QPN)/ports/qt/qpn_app.cpp \
    $(QPN)/ports/qt/pixellabel.cpp \
    main.cpp \
    gui.cpp \
    bsp.cpp \
    missile.c \
    mine2.c \
    mine1.c \
    tunnel.c \
    ship.c

FORMS += gui.ui

RESOURCES = gui.qrc

RESOURCES = gui.qrc

win32:RC_FILE = gui.rc
