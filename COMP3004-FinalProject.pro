QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    battery.cpp \
    createprofilewindow.cpp \
    device.cpp \
    historyviewer.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    note.cpp \
    profilewindow.cpp \
    readingstorage.cpp \
    recommendation.cpp \
    user.cpp

HEADERS += \
    battery.h \
    createprofilewindow.h \
    defs.h \
    device.h \
    historyviewer.h \
    loginwindow.h \
    mainwindow.h \
    note.h \
    profilewindow.h \
    readingstorage.h \
    recommendation.h \
    user.h

FORMS += \
    createprofilewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    profilewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
