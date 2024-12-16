cmake_minimum_required(VERSION 3.10)

project(Client)

set(CMAKE_CXX_STANDARD 11)

# Set the path to your Qt installation
set(CMAKE_PREFIX_PATH "/home/voidkeishi/Qt/6.8.1/gcc_64")

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6.8 components
find_package(Qt6 6.8.1 COMPONENTS Gui Core Network Quick Qml Sql REQUIRED)
find_package(SQLite3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# QML resources
qt_add_resources(QML_RESOURCES
    resources.qrc
)

# Create executable
add_executable(Client 
    ${SOURCES}
    ${QML_RESOURCES}
)

# Set include directories
target_include_directories(Client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt6Gui_INCLUDE_DIRS}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Quick_INCLUDE_DIRS}
    ${Qt6Qml_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Client PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Quick
    Qt6::Qml
    Qt6::Sql
    SQLite::SQLite3
)