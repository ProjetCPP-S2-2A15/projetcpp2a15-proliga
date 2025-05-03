QT       += core gui
QT += sql
QT += widgets sql
QT += printsupport
QT += core gui sql network
QT += network
QT += widgets
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
    arbitre.cpp \
    changeWidget.cpp \
     connexion.cpp \
    design.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    arbitre.h \
    changeWidget.h \
      loginwindow.h \
     connexion.h\
    design.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
