cmake_minimum_required(VERSION 3.0)
project(IOB_Server)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library lists the components needed
find_package(Qt5 REQUIRED COMPONENTS core widgets xml websockets)

# Set source files
set( SOURCES
  main.cpp
  server_iob.cpp
  client.cpp
)

# Set header files
set( HEADERS
  server_iob.h
  client.h
)

# set ui elements
set (UIS
  server_iob.ui
)

# Set resource files
set( RESOURCES
  server_iob.qrc
)

# directories searched for includ files
include_directories( ${CMAKE_BINARY_DIR} )

# Tell CMake to create the iob_server executable
add_executable(iob_server main.cpp ${HEADERS} ${SOURCES} ${UIS} ${RESOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(iob_server Qt5::Widgets)
target_link_libraries(iob_server Qt5::WebSockets)
target_link_libraries(iob_server Qt5::Xml)