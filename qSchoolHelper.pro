TEMPLATE = app
TARGET = qSchoolHelper
INCLUDEPATH += include include/curl .
UI_DIR = ui
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION_MAJOR = 0
VERSION_MINOR = 5
VERSION_BUILD = 0
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

CONFIG += c++17 lrelease embed_translations

DEFINES += \
    "VERSION_MAJOR=$$VERSION_MAJOR" \
    "VERSION_MINOR=$$VERSION_MINOR" \
    "VERSION_BUILD=$$VERSION_BUILD" \
    "APP_VERSION=\\\"$$VERSION\\\""


SOURCES += \
    src/AboutDialog.cpp \
    src/CleaningDialog.cpp \
    src/HelpDialog.cpp \
    src/InitialSetupDialog.cpp \
    src/InstallDialog.cpp \
    src/MainWindow.cpp \
    src/ProblemDialog.cpp \
    src/Procedure.cpp \
    src/main.cpp

HEADERS += \
    include/AboutDialog.h \
    include/CleaningDialog.h \
    include/HelpDialog.h \
    include/InitialSetupDialog.h \
    include/InstallDialog.h \
    include/MainWindow.h \
    include/ProblemDialog.h \
    include/Procedure.h

FORMS += \
    ui/AboutDialog.ui \
    ui/CleaningDialog.ui \
    ui/HelpDialog.ui \
    ui/InitialSetupDialog.ui \
    ui/InstallDialog.ui \
    ui/MainWindow.ui \
    ui/ProblemDialog.ui


TRANSLATIONS += \
    i18n/qSchoolHelper_pl_PL.ts

RC_ICONS = data/icons/icon.ico
RESOURCES = ui/resources.qrc

include (include/qtcurl/src/qtcurl.pri)

QMAKE_LFLAGS += -lcurl
LIBS += -lcurl

# Include manifest
win32:CONFIG(release, debug|release): QMAKE_MANIFEST = $$PWD/data/qSchoolHelper.manifest.xml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lcurl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibcurl.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibcurl.dll
