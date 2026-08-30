cmake_minimum_required(VERSION 3.16)
project(defencute DESCRIPTION "Qt based simple TowerDefence game example")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(UNIX)
    # Qt6 installed as a package
else()
    set(CMAKE_PREFIX_PATH "$ENV{QT6_PATH}//")
endif()
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(qtgame)

set(src_top "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(qtgame_src
    ${src_top}/common/common.h
    ${src_top}/common/IFace.h
    ${src_top}/common/ICoreObject.h
    ${src_top}/common/ICoreClass.h
    ${src_top}/common/ICore.h
    ${src_top}/common/ITile.h
    ${src_top}/common/ITower.h
    ${src_top}/common/IEnemy.h
    ${src_top}/common/IScene.h
    ${src_top}/core.h
    ${src_top}/core.cpp
    ${src_top}/projectile.h
    ${src_top}/projectile.cpp
    ${src_top}/towermenubtn.h
    ${src_top}/towermenubtn.cpp
    ${src_top}/towermenu.h
    ${src_top}/towermenu.cpp
    ${src_top}/tower.h
    ${src_top}/tower.cpp
    ${src_top}/enemy.h
    ${src_top}/enemy.cpp
    ${src_top}/hexmenubtn.h
    ${src_top}/hexmenubtn.cpp
    ${src_top}/hexmenu.h
    ${src_top}/hexmenu.cpp
    ${src_top}/hextile.h
    ${src_top}/hextile.cpp
    ${src_top}/InfoPanel.h
    ${src_top}/InfoPanel.cpp
    ${src_top}/SceneGeneric.h
    ${src_top}/SceneGeneric.cpp
    ${src_top}/GameView.h
    ${src_top}/GameView.cpp
    ${src_top}/GameController.h
    ${src_top}/GameController.cpp
    ${src_top}/MainWindow.h
    ${src_top}/MainWindow.cpp
    ${src_top}/main.cpp
    defencute.qrc
)

target_sources(qtgame PRIVATE
    ${qtgame_src}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${qtgame_src})

target_include_directories(qtgame PRIVATE
    ${src_top}/common
    ${CMAKE_PREFIX_PATH}include
    ${CMAKE_PREFIX_PATH}include/QtGui
)

target_link_libraries(qtgame Qt::Core Qt::Gui Qt::Widgets)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT qtgame)


 if(WIN32)
    # Locate the windeployqt tool based on your Qt6 targets
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        # Run windeployqt after every successful build to copy missing DLLs
        add_custom_command(TARGET qtgame POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-compiler-runtime
                    --verbose 1
                    "$<TARGET_FILE:qtgame>"
            COMMENT "Deploying Qt DLLs to build directory using windeployqt..."
        )
    endif()
endif()