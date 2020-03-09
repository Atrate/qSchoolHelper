TEMPLATE = app
TARGET = qSchoolHelper
INCLUDEPATH += include .
UI_DIR = ui
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/main_window.cpp

HEADERS += \
    include/main_window.h

FORMS += \
    ui/main_window.ui \
    ui/about_dialog.ui \
    ui/cleaning_dialog.ui \
    ui/initial_setup_dialog.ui \
    ui/install_dialog.ui \
    ui/problem_dialog.ui


TRANSLATIONS += \
    data/translations/qSchoolHelper_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
