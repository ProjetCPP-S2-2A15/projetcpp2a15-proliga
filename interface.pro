QT       += core gui
QT += sql
QT += widgets sql

QT += core gui printsupport
QT += charts
QT += sql charts
QT += network
QT += core gui sql network
QT += concurrent
QT += multimedia  # Change to your Python version  # Change '3.x' to your Python version

QT += widgets

QT += axcontainer
QT += printsupport

QT += serialport
QT += quick location positioning
QT += quickwidgets
QT += positioning location
QT += quick positioning location


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    equipe.cpp \
    statistique.cpp \
    arbitre.cpp \
    arduinoshiraz.cpp \
    championnats.cpp \
    changeWidget.cpp \
     connexion.cpp \
    design.cpp \
    loginwindow.cpp \
    main.cpp \
    match.cpp \
    scene1.cpp \
    statisticscontainer.cpp\
    stades.cpp \
    mainwindow.cpp
HEADERS += \
    arbitre.h \
    arduinoshiraz.h \
    championnats.h \
    changeWidget.h \
    loginwindow.h \
    connexion.h\
    design.h \
    mainwindow.h \
    statistique.h \
    equipe.h \
    match.h \
    scene1.h \
    statisticscontainer.h \
    stades.h
FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

