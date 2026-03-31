cmake_minimum_required(VERSION 3.16)
project(MedLexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_PACKAGE Qt5)
endif()

set(SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/core/Token.cpp
    src/core/LexicalAnalyzer.cpp
    src/core/ErrorManager.cpp
)

set(HEADERS
    src/gui/MainWindow.h
    src/core/Token.h
    src/core/LexicalAnalyzer.h
    src/core/ErrorManager.h
)

add_executable(MedLexer ${SOURCES} ${HEADERS})
target_include_directories(MedLexer PRIVATE src)
target_link_libraries(MedLexer PRIVATE ${QT_PACKAGE}::Widgets)
