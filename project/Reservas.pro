#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T21:10:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Reservas
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


SOURCES += \
        main.cpp \
    loginform.cpp \
    clientform.cpp \
    logregform.cpp \
    registerform.cpp \
    clubforms.cpp

HEADERS += \
    loginform.h \
    registerform.h \
    clientform.h \
    logregform.h \
    serverparams.h \
    clientsessionmanager.h \
    clubsessionmanager.h \
    clubforms.h

FORMS += \
    loginform.ui \
    registerform.ui \
    clientform.ui \
    logregform.ui \
    clubforms.ui

RESOURCES += \
    img.qrc

DISTFILES += \
    img/login1.jpg
