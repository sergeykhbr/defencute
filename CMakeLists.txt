cmake_minimum_required(VERSION 3.4.0)
project(defencute DESCRIPTION "Qt based simple TowerDefence game example")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "$ENV{QT_PATH}//")
find_package(Qt6 REQUIRED COMPONENTS Core Gui StateMachine Widgets)

set(src_top "${CMAKE_CURRENT_SOURCE_DIR}/src")

# It is neccessary to include QtGui to workaroung error in moc_ file
include_directories(
    ${src_top}/common
    ${CMAKE_PREFIX_PATH}include
    ${CMAKE_PREFIX_PATH}include/QtGui
)


file(GLOB_RECURSE qtgame_src
    LIST_DIRECTORIES false
    ${src_top}/*.cpp
    ${src_top}/*.h
    ${src_top}/*.qrc
)

add_executable(
   qtgame
   ${qtgame_src}
)

if(UNIX)
    set(LIBRARY_OUTPUT_PATH "bin")
    add_compile_options(-std=c++17)

    target_link_libraries(qtgame pthread rt dl Qt::Core Qt::Gui Qt::Widgets Qt::StateMachine)
    add_custom_command(TARGET qtgame
	        POST_BUILD
	        COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/post_build_event.sh "${CMAKE_CURRENT_BINARY_DIR}/bin" "${CMAKE_CURRENT_SOURCE_DIR}"
	        )

else()

    foreach(_source IN ITEMS ${qtgame_src})
        get_filename_component(_source_path "${_source}" PATH)
        # form proper filter name from the path to source file:
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        string(REPLACE "cmake\\qtgame" "" _group_path "${_group_path}")
 
        source_group("${_group_path}" FILES "${_source}")
    endforeach()

    set_target_properties(qtgame PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")
    set_target_properties(qtgame PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
    set_target_properties(qtgame PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT qtgame)

    target_link_libraries(qtgame Qt::Core Qt::Gui Qt::Widgets Qt::StateMachine)
    add_custom_command(TARGET qtgame 
	        POST_BUILD
	        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/post_build_event.bat "\"$(SolutionDir)bin\""
	        )

endif()

 