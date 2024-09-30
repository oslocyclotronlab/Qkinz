cmake_minimum_required(VERSION 3.21)
project(Qkinz VERSION 1.4.0 LANGUAGES C CXX)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")

find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets WebEngine WebEngineCore WebEngineWidgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

# ---- Add source files ----
set(headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/qcustomplot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/rundialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/selectbeamform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/selectfrontbackform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/selecttargetform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/include/selecttelescopeform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/BetheBlock.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/BetheBlockComp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/CustomPower.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/DickNorbury.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/FileSP.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/Iterative.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/LNScattering.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/RelScatter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/Scattering.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/StoppingPower.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/Ziegler1985.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/include/ZieglerComp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/AbstractFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/PolyD2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/Polyfit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/Polynomial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/spline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/include/Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matter/include/Material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matter/include/Particle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/include/BatchReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/include/Runsystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/include/tablemakerhtml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/include/worker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/include/ame2012_mass_tables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/include/ame2012_masses.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/include/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/include/ziegler1985_table.h
)
set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/qcustomplot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/rundialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/selectbeamform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/selectfrontbackform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/selecttargetform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/src/selecttelescopeform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/BetheBlock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/BetheBlockComp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/CustomPower.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/DickNorbury.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/FileSP.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/Iterative.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/LNScattering.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/RelScatter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/Scattering.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/StoppingPower.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/Ziegler1985.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kinematics/src/ZieglerComp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/AbstractFunction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/Matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/PolyD2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/Polyfit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/Polynomial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/spline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/src/Vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matter/src/Material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matter/src/Particle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/src/BatchReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/src/Runsystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/src/tablemakerhtml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/support/src/worker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/src/ame2012_mass_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/src/ame2012_masses.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/src/excitation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tables/src/ziegler1985_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main.cpp
)

qt5_add_resources(sources resources/resorces.qrc)

add_executable(${PROJECT_NAME} ${headers} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/gui/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/kinematics/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/math/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/matter/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/support/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/tables/include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Widgets
        Qt5::WebEngineCore
        Qt5::WebEngine
        Qt5::WebEngineWidgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)