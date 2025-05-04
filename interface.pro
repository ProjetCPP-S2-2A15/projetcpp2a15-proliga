QT       += core gui
QT += sql
QT += widgets sql
QT += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts
QT += sql charts
QT += network
QT += core gui sql network
QT += concurrent
QT += multimedia  # Change to your Python version  # Change '3.x' to your Python version
TARGET = interface
TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changeWidget.cpp \
     connexion.cpp \
    design.cpp \
    equipe.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    statistique.cpp

HEADERS += \
    changeWidget.h \
    equipe.h \
      loginwindow.h \
     connexion.h\
    design.h \
    mainwindow.h \
    statistique.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

DISTFILES +=
