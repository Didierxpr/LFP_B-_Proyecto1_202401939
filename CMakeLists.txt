cmake_minimum_required(VERSION 3.16)
project(MedLexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  NECESARIO PARA QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  IMPORTANTE: ayuda a encontrar Qt automáticamente
set(CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH})

# Buscar Qt6 primero, si no Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)

if(Qt6_FOUND)
    message(STATUS "Usando Qt6")
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    message(STATUS "Usando Qt5")
    set(QT_PACKAGE Qt5)
endif()

#  ARCHIVOS
set(SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/core/Token.cpp
    src/core/LexicalAnalyzer.cpp
    src/core/ErrorManager.cpp
    src/core/MedLangParser.cpp
)

set(HEADERS
    src/gui/MainWindow.h
    src/core/Token.h
    src/core/LexicalAnalyzer.h
    src/core/ErrorManager.h
)


add_executable(MedLexer
    src/main.cpp
    src/gui/MainWindow.cpp
    src/core/Token.cpp
    src/core/LexicalAnalyzer.cpp
    src/core/ErrorManager.cpp
    src/core/MedLangParser.cpp
)

target_include_directories(MedLexer PRIVATE src)

target_link_libraries(MedLexer PRIVATE ${QT_PACKAGE}::Widgets)
