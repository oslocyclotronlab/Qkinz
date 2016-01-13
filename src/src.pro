#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T18:08:06
#
#-------------------------------------------------
include(../defaults.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = kinzgui
TEMPLATE = lib
CONFIG += staticlib


DESTDIR = $$BUILDDIR/src
OBJECTS_DIR = $$BUILDDIR/src/objects
MOC_DIR = $$BUILDDIR/src/moc
RCC_DIR = $$BUILDDIR/src/rcc
UI_DIR = $$BUILDDIR/src/ui

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -std=c++11 #-O3
    LIBS += -dead_strip  #-stdlib=libc++
#    ICON = ../resources/media/Qkinz.icns
}
QMAKE_CXXFLAGS += -std=c++11

SOURCES +=  gui/src/mainwindow.cpp \
            gui/src/selectbeamform.cpp \
            gui/src/qcustomplot.cpp \
            gui/src/rundialog.cpp \
            gui/src/selecttargetform.cpp \
            gui/src/selecttelescopeform.cpp \
            gui/src/selectfrontbackform.cpp \
            kinematics/src/BetheBlock.cpp \
            kinematics/src/BetheBlockComp.cpp \
            kinematics/src/DickNorbury.cpp \
            kinematics/src/FileSP.cpp \
            kinematics/src/Iterative.cpp \
            kinematics/src/Scattering.cpp \
            kinematics/src/LNScattering.cpp \
            kinematics/src/StoppingPower.cpp \
            kinematics/src/Ziegler1985.cpp \
            kinematics/src/ZieglerComp.cpp \
            math/src/AbstractFunction.cpp \
            math/src/Matrix.cpp \
            math/src/PolyD2.cpp \
            math/src/Polyfit.cpp \
            math/src/Polynomial.cpp \
            math/src/spline.cpp \
            math/src/Vector.cpp \
            matter/src/Material.cpp \
            matter/src/Particle.cpp \
            support/src/runsystem.cpp \
            support/src/tablemakerhtml.cpp \
            support/src/worker.cpp \
            tables/src/ame2012_mass_tables.cpp \
            tables/src/ame2012_masses.cpp \
            tables/src/ziegler1985_table.cpp \
            tables/src/excitation.cpp
            

HEADERS  += gui/include/mainwindow.h \
            gui/include/selectbeamform.h \
            gui/include/qcustomplot.h \
            gui/include/rundialog.h \
            gui/include/selecttargetform.h \
            gui/include/selecttelescopeform.h \
            gui/include/selectfrontbackform.h \
            kinematics/include/BetheBlock.h \
            kinematics/include/BetheBlockComp.h \
            kinematics/include/DickNorbury.h \
            kinematics/include/FileSP.h \
            kinematics/include/Iterative.h \
            kinematics/include/Scattering.h \
            kinematics/include/LNScattering.h \
            kinematics/include/StoppingPower.h \
            kinematics/include/Ziegler1985.h \
            kinematics/include/ZieglerComp.h \
            math/include/AbstractFunction.h \
            math/include/Matrix.h \
            math/include/PolyD2.h \
            math/include/Polyfit.h \
            math/include/Polynomial.h \
            math/include/spline.h \
            math/include/Vector.h \
            matter/include/Material.h \
            matter/include/Particle.h \
            support/include/runsystem.h \
            support/include/tablemakerhtml.h \
            support/include/worker.h \
            tables/include/ame2012_mass_tables.h \
            tables/include/ame2012_masses.h \
            tables/include/ziegler1985_table.h \
            tables/include/excitation.h \
            global.h \
            types.h

FORMS    += gui/forms/mainwindow.ui \
            gui/forms/selectbeamform.ui \
            gui/forms/selecttargetform.ui \
            gui/forms/selecttelescopeform.ui \
            gui/forms/rundialog.ui \
            gui/forms/selectfrontbackform.ui

RESOURCES += ../resources/resorces.qrc

QMAKE_CLEAN +=  $$BUILDDIR/src/objects/*.o \
                $$BUILDDIR/src/moc/moc_* \
                $$BUILDDIR/src/rcc/qrc_* \
                $$BUILDDIR/src/ui/ui_*
