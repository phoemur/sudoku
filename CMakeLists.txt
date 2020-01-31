cmake_minimum_required(VERSION 3.9)

project(sudoku DESCRIPTION "Simple Sudoku Game" LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/3rdParty/NamedType"
)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    LinguistTools
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

#l10n
#qt5_create_translation(QM_FILES ${HEADERS} ${SOURCES} ${FORMS} ${CMAKE_SOURCE_DIR}/l10n/translation_pt.ts)
qt5_add_translation(QM_FILES ${CMAKE_SOURCE_DIR}/l10n/translation_pt.ts)

# Further HEADERS
set(HEADERS ${HEADERS} include/my_types.h 3rdParty/NamedType/named_type.hpp)

# Compile / translate
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${sudoku_MOC} ${sudoku_UI} ${RESOURCES_ADDED} ${QM_FILES})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        $<$<CONFIG:Debug>:
            -O0 -Wall -Wextra -Werror -pedantic-errors -g>
        $<$<CONFIG:Release>:
            -O3 -Wall -pipe -march=native>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

# Link
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Widgets)

#Install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR})

