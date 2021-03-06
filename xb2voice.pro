# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
VERSION = 1.2.0
TARGET = xb2voice
DESTDIR = ./release
QT += core network gui multimedia widgets
CONFIG += release
DEFINES += _WINDOWS _UNICODE WIN64 QT_MULTIMEDIA_LIB QT_WIDGETS_LIB QT_NETWORK_LIB
INCLUDEPATH += ./GeneratedFiles/$(ConfigurationName) \
    ./GeneratedFiles \
    . \
    ./release \
    $(QTDIR)/mkspecs/win32-msvc
LIBS += -lshell32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(xb2voice.pri)

DISTFILES +=
RC_ICONS = ./assets/icon/icon.ico
