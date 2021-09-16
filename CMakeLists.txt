
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(Layers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRC
    ${PROJECT_SOURCE_DIR}/const.h
    ${PROJECT_SOURCE_DIR}/Layers.cpp
    ${PROJECT_SOURCE_DIR}/Layers.h
    ${PROJECT_SOURCE_DIR}/Layers.ui
    ${PROJECT_SOURCE_DIR}/LayListModel.cpp
    ${PROJECT_SOURCE_DIR}/LayListModel.h
    ${PROJECT_SOURCE_DIR}/MratModel.cpp
    ${PROJECT_SOURCE_DIR}/MratModel.h
    ${PROJECT_SOURCE_DIR}/Paths.h
    ${PROJECT_SOURCE_DIR}/PeriodicTable.cpp
    ${PROJECT_SOURCE_DIR}/PeriodicTable.h
    ${PROJECT_SOURCE_DIR}/PeriodicTable.ui
	${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
