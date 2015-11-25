#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T18:08:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = Qkinz
TEMPLATE = app

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -std=c++11 #-O3
    LIBS += -dead_strip  #-stdlib=libc++
    ICON = resources/media/Qkinz.icns
}


INCLUDEPATH +=  source \
                source/gui/include \
                source/kinematics/include \
                source/math/include \
                source/matter/include \
                source/support/include \
                source/tables/include

SOURCES +=  source/main.cpp \
            source/gui/src/mainwindow.cpp \
            source/gui/src/selectbeamform.cpp \
            source/gui/src/qcustomplot.cpp \
            source/gui/src/rundialog.cpp \
            source/gui/src/selecttargetform.cpp \
            source/gui/src/selecttelescopeform.cpp \
            source/kinematics/src/BetheBlock.cpp \
            source/kinematics/src/BetheBlockComp.cpp \
            source/kinematics/src/DickNorbury.cpp \
            source/kinematics/src/FileSP.cpp \
            source/kinematics/src/Iterative.cpp \
            source/kinematics/src/Scattering.cpp \
            source/kinematics/src/StoppingPower.cpp \
            source/kinematics/src/Ziegler1985.cpp \
            source/kinematics/src/ZieglerComp.cpp \
            source/math/src/AbstractFunction.cpp \
            source/math/src/Matrix.cpp \
            source/math/src/PolyD2.cpp \
            source/math/src/Polyfit.cpp \
            source/math/src/Polynomial.cpp \
            source/math/src/spline.cpp \
            source/math/src/Vector.cpp \
            source/matter/src/Material.cpp \
            source/matter/src/Particle.cpp \
            source/support/src/runsystem.cpp \
            source/support/src/tablemakerhtml.cpp \
            source/support/src/worker.cpp \
            source/tables/src/ame2012_mass_tables.cpp \
            source/tables/src/ame2012_masses.cpp \
            source/tables/src/ziegler1985_table.cpp \
            source/tables/src/excitation.cpp
            

HEADERS  += source/gui/include/mainwindow.h \
            source/gui/include/selectbeamform.h \
            source/gui/include/qcustomplot.h \
            source/gui/include/rundialog.h \
            source/gui/include/selecttargetform.h \
            source/gui/include/selecttelescopeform.h \
            source/kinematics/include/BetheBlock.h \
            source/kinematics/include/BetheBlockComp.h \
            source/kinematics/include/DickNorbury.h \
            source/kinematics/include/FileSP.h \
            source/kinematics/include/Iterative.h \
            source/kinematics/include/Scattering.h \
            source/kinematics/include/StoppingPower.h \
            source/kinematics/include/Ziegler1985.h \
            source/kinematics/include/ZieglerComp.h \
            source/math/include/AbstractFunction.h \
            source/math/include/Matrix.h \
            source/math/include/PolyD2.h \
            source/math/include/Polyfit.h \
            source/math/include/Polynomial.h \
            source/math/include/spline.h \
            source/math/include/Vector.h \
            source/matter/include/Material.h \
            source/matter/include/Particle.h \
            source/support/include/runsystem.h \
            source/support/include/tablemakerhtml.h \
            source/support/include/worker.h \
            source/tables/include/ame2012_mass_tables.h \
            source/tables/include/ame2012_masses.h \
            source/tables/include/ziegler1985_table.h \
            source/tables/include/excitation.h \
            source/global.h \
            source/types.h 
            

FORMS    += source/gui/forms/mainwindow.ui \
            source/gui/forms/selectbeamform.ui \
            source/gui/forms/selecttargetform.ui \
            source/gui/forms/selecttelescopeform.ui \
            source/gui/forms/rundialog.ui

RESOURCES += resources/resorces.qrc
