cmake_minimum_required(VERSION 3.9)

project(sudoku DESCRIPTION "Simple Sudoku Game" LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${CMAKE_SOURCE_DIR}/include")

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
)

# Sources
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mylineedit.cpp
    src/solver.hpp
)

# Headers
set(HEADERS
    include/mainwindow.h
    include/mylineedit.h
)

# Forms
set(FORMS
    forms/mainwindow.ui
)

# UI, MOC and resources
qt5_add_resources(RESOURCES_ADDED "${CMAKE_SOURCE_DIR}/assets/assets.qrc")
qt5_wrap_ui(sudoku_UI ${FORMS})
qt5_wrap_cpp(sudoku_MOC ${HEADERS})

# Compile / translate
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${sudoku_MOC} ${sudoku_UI} ${RESOURCES_ADDED})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        $<$<CONFIG:Debug>:
            -O0 -Wall -Wextra -Weffc++ -Werror -pedantic-errors -g>
        $<$<CONFIG:Release>:
            -O3 -Wall -pipe -march=native>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

# Link
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Widgets)

#Install
install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

