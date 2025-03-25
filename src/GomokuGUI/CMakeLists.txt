cmake_minimum_required(VERSION 3.10)
project(GomokuGUI)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_wrap_cpp(MOC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/GomokuGUI/MainWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/GomokuGUI/BoardWidget.h
)

add_executable(GomokuGUI
    main.cpp
    MainWindow.cpp
    BoardWidget.cpp
    ${MOC_SRC}
)

target_include_directories(GomokuGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(GomokuGUI
    Qt5::Widgets
    GomokuLib
)