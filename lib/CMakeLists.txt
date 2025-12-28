cmake_minimum_required(VERSION 3.16)
project(juneserverlib)

set(CMAKE_CXX_STANDARD 20)

set_target_properties(PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

find_package(Qt6 COMPONENTS WebSockets REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS HttpServer REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

qt_add_library(juneserverlib
    app.h
    app.cpp
    client.h
    client.cpp
    clientinformation.h
    clientinformation.cpp
    logger.h
    logger.cpp
    logvaluedata.h
    logvaluedata.cpp
    pluginmanager.h
    pluginmanager.cpp
    websocketserver.h
    websocketserver.cpp

    api/logvalueapi.h
    api/logvalueapi.cpp
    api/pluginapi.h
    api/pluginapi.cpp
    api/tagapi.h
    api/tagapi.cpp
    api/tagsocketbindingapi.h
    api/tagsocketbindingapi.cpp
    api/triggerapi.h
    api/triggerapi.cpp

    automation/automationmanager.h
    automation/automationmanager.cpp
    automation/trigger.h
    automation/trigger.cpp
    automation/triggerfactory.h
    automation/trigger/triggereverytimeabove.h
    automation/trigger/triggereverytimeabove.cpp
    automation/trigger/triggereverytimebelow.h
    automation/trigger/triggereverytimebelow.cpp
    automation/trigger/triggermultivalue.h
    automation/trigger/triggermultivalue.cpp
    automation/trigger/triggerontime.h
    automation/trigger/triggerontime.cpp
    automation/trigger/triggerschedule.h
    automation/trigger/triggerschedule.cpp
    automation/trigger/triggertwovalues.h
    automation/trigger/triggertwovalues.cpp
    automation/trigger/valueitem.h
    automation/trigger/valueitem.cpp
)

target_link_libraries(juneserverlib PRIVATE Qt6::WebSockets
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::HttpServer
    Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(juneserverlib PRIVATE tagsystemd)
    target_link_libraries(juneserverlib PRIVATE influxdbd)
    target_link_libraries(juneserverlib PRIVATE pluginLoadd)
    target_link_libraries(juneserverlib PRIVATE cored)
else()
    message("Release mode")
    target_link_libraries(juneserverlib PRIVATE tagsystem)
    target_link_libraries(juneserverlib PRIVATE influxdb)
    target_link_libraries(juneserverlib PRIVATE pluginLoad)
    target_link_libraries(juneserverlib PRIVATE core)
endif()

target_include_directories(juneserverlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


set_target_properties(juneserverlib
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

