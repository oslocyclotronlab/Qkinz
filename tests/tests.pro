include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle

DESTDIR = $$BUILDDIR/tests
OBJECTS_DIR = $$BUILDDIR/tests
MOC_DIR = $$BUILDDIR/tests
RCC_DIR = $$BUILDDIR/tests
UI_DIR = $$BUILDDIR/tests

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -std=c++11 #-O3
    LIBS += -dead_strip  #-stdlib=libc++
    #ICON = ../resources/media/Qkinz.icns
}

SOURCES += main.cpp

LIBS += -L$$BUILDDIR/src -lkinzgui

HEADERS += catch.hpp

QMAKE_CLEAN += 	$$BUILDDIR/tests/*.o \
				$$BUILDDIR/tests/moc_* \
				$$BUILDDIR/tests/qrc_* \
				$$BUILDDIR/tests/ui_*