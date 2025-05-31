cmake_minimum_required(VERSION 3.16)
project(juneserver)

set(CMAKE_CXX_STANDARD 20)

set_target_properties(PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

include_directories($ENV{JUNE_ROOT})
link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

add_definitions(-DNO_GUI)

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

find_package(Qt6 COMPONENTS WebSockets REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS HttpServer REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

qt_add_executable(juneserver
    main.cpp
    addlogvaluedialog.cpp
    app.cpp
    logvaluedata.cpp
    logvaluemodel.cpp
    logvalueview.cpp
    pluginmanager.cpp
    websocketserver.cpp
    mainwindow.cpp
    gui/menubar.cpp
    gui/clientlistwidget.cpp
    client.cpp
    clientinformation.cpp
    logger.cpp
    gui/loggerwidget.cpp

    addlogvaluedialog.h
    app.h
    logvaluedata.h
    logvaluemodel.h
    logvalueview.h
    pluginmanager.h
    websocketserver.h
    mainwindow.h
    gui/menubar.h
    gui/clientlistwidget.h
    client.h
    clientinformation.h
    logger.h
    gui/loggerwidget.h

    addlogvaluedialog.ui
    logvalueview.ui
    mainwindow.ui

    api/pluginapi.h
    api/pluginapi.cpp
    api/tagapi.h
    api/tagapi.cpp

    automation/automationmanager.h
    automation/automationmanager.cpp
    automation/trigger.h
    automation/trigger.cpp
    automation/triggerfactory.h
    automation/trigger/triggereverytimeabove.h
    automation/trigger/triggereverytimeabove.cpp
    automation/trigger/triggereverytimebelow.h
    automation/trigger/triggereverytimebelow.cpp
    automation/trigger/triggerontime.h
    automation/trigger/triggerontime.cpp
    api/triggerapi.h api/triggerapi.cpp

    util/util.h
    util/util.cpp
)

qt_add_resources(juneserver resources
    PREFIX "/resources"
    FILES resources/icon.png
)

target_link_libraries(juneserver PRIVATE Qt6::WebSockets
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::HttpServer
    Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(juneserver PRIVATE tagsystemd)
    target_link_libraries(juneserver PRIVATE influxdbd)
    target_link_libraries(juneserver PRIVATE pluginLoadd)
    target_link_libraries(juneserver PRIVATE cored)
else()
    message("Release mode")
    target_link_libraries(juneserver PRIVATE tagsystem)
    target_link_libraries(juneserver PRIVATE influxdb)
    target_link_libraries(juneserver PRIVATE pluginLoad)
    target_link_libraries(juneserver PRIVATE core)
endif()

target_include_directories(juneserver INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


set_target_properties(juneserver
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


