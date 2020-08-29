TEMPLATE = app
TARGET = qSchoolHelper
INCLUDEPATH += include include/curl .
UI_DIR = ui
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

CONFIG += c++17 lrelease embed_translations

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x060000 \
    "VERSION_MAJOR=$$VERSION_MAJOR" \
    "VERSION_MINOR=$$VERSION_MINOR" \
    "VERSION_BUILD=$$VERSION_BUILD" \
    "APP_VERSION=\\\"$$VERSION\\\""


SOURCES += \
    src/about_dialog.cpp \
    src/cleaning_dialog.cpp \
    src/help_dialog.cpp \
    src/initial_setup_dialog.cpp \
    src/install_dialog.cpp \
    src/main.cpp \
    src/main_window.cpp \
    src/problem_dialog.cpp \
    src/procedures.cpp

HEADERS += \
    include/about_dialog.h \
    include/cleaning_dialog.h \
    include/help_dialog.h \
    include/initial_setup_dialog.h \
    include/install_dialog.h \
    include/main_window.h \
    include/problem_dialog.h \
    include/procedures.h

FORMS += \
    ui/about_dialog.ui \
    ui/cleaning_dialog.ui \
    ui/help_dialog.ui \
    ui/initial_setup_dialog.ui \
    ui/install_dialog.ui \
    ui/main_window.ui \
    ui/problem_dialog.ui


TRANSLATIONS += \
    i18n/qSchoolHelper_pl_PL.ts

RC_ICONS = data/icons/icon.ico
RESOURCES = ui/resources.qrc

include (include/qtcurl/src/qtcurl.pri)

QMAKE_LFLAGS += -lcurl
LIBS += -lcurl

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lcurl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibcurl.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibcurl.dll
