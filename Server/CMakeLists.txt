cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_CXX_STANDARD 11)

# Set the path to your Qt installation
set(CMAKE_PREFIX_PATH "/home/voidkeishi/Qt/6.8.1/gcc_64")

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6.8 components
find_package(Qt6 6.8.1 COMPONENTS Core Network Sql REQUIRED)
find_package(SQLite3 REQUIRED)

# Collect source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Add executable (define the target)
add_executable(Server ${SOURCES} ${HEADERS})

# Include directories (modify the target after it's defined)
target_include_directories(Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Network_INCLUDE_DIRS}
    ${Qt6Sql_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Server PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    SQLite::SQLite3
)