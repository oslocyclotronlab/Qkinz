include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -std=c++11 #-O3
    LIBS += -dead_strip  #-stdlib=libc++
    #ICON = ../resources/media/Qkinz.icns
}

SOURCES += main.cpp

LIBS += -L../src -lkinzgui

HEADERS += \
    catch.hpp
