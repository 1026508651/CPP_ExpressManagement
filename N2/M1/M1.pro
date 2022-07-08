#-------------------------------------------------
#
# Project created by QtCreator 2022-05-02T16:45:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = M1
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
    registerwindow.cpp \
    login.cpp \
    navigation.cpp \
    admincheck.cpp \
    pkgsend.cpp \
    pkgrev.cpp \
    pkgsearch.cpp \
    pocket.cpp \
    safety.cpp \
    adminpkgsearch.cpp \
    adminusercheck.cpp \
    pkgmanrecv.cpp \
    pkgmannavi.cpp \
    pkgmancheck.cpp \
    pkgmanregister.cpp \
    userssafty.cpp


HEADERS += \
    login.h \
    registerwindow.h \
    basic.h \
    variables.h \
    navigation.h \
    admincheck.h \
    pkgsend.h \
    pkgrev.h \
    pkgsearch.h \
    pocket.h \
    safety.h \
    adminpkgsearch.h \
    adminusercheck.h \
    pkgmanrecv.h \
    pkgmannavi.h \
    pkgmancheck.h \
    pkgmanregister.h \
    userssafty.h

FORMS += \
        login.ui \
    registerwindow.ui \
    navigation.ui \
    admincheck.ui \
    pkgsend.ui \
    pkgrev.ui \
    pkgsearch.ui \
    pocket.ui \
    safety.ui \
    adminpkgsearch.ui \
    adminusercheck.ui \
    pkgmanrecv.ui \
    pkgmannavi.ui \
    pkgmancheck.ui \
    pkgmanregister.ui \
    userssafty.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mainres.qrc
