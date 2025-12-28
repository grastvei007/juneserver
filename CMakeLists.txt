cmake_minimum_required(VERSION 3.16)
project(juneserver)

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

qt_add_executable(juneserver
    main.cpp
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
    target_link_libraries(juneserver PRIVATE juneserverlibd)
else()
    message("Release mode")
    target_link_libraries(juneserver PRIVATE tagsystem)
    target_link_libraries(juneserver PRIVATE influxdb)
    target_link_libraries(juneserver PRIVATE pluginLoad)
    target_link_libraries(juneserver PRIVATE core)
    target_link_libraries(juneserver PRIVATE juneserverlib)
endif()

target_include_directories(juneserver INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(juneserver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)


set_target_properties(juneserver
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

add_subdirectory(lib)
add_subdirectory(tests)

add_dependencies(juneserverlib tagsystem influxdb pluginLoad core)

