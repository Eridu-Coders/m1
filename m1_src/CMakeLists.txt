cmake_minimum_required(VERSION 3.16)

project(m1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WebEngineWidgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(PROJECT_SOURCES
        main.cpp
        m1A_constants.h
        m1A_env.h
        m1A_env.cpp
        m1B_graph_init.h
        m1B_graph_init.cpp
        m1B_store.h
        m1B_store.cpp
        m1B_store_mmap.cpp
        m1B_Store_debug.cpp
        m1B_store_load_save.cpp
        m1B_lv0_item.h
        m1B_lv0_item.cpp
        m1B_lv0_item_members.cpp
        m1B_lv1_item.h
        m1B_lv1_item.cpp
        m1B_lv2_item.h
        m1B_lv2_item.cpp
        m1B_lv2_item_instantiation.cpp
        m1B_lv2_item_debug.cpp
        m1B_lv2_iterators.cpp
        m1B_lv2_item_linking.cpp
        m1B_lv2_item_types.cpp
        m1C_interp.h
        m1C_interp.cpp
        m1D_main_window.h
        m1D_main_window.cpp
        m1D_tree_display.h
        m1D_tree_display.cpp
        m1D_passages_panel.h
        m1D_passages_panel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(m1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        m1B_lv2_iterators.h
        m1C_interp_new.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET m1_src APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(m1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(m1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(m1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::WebEngineWidgets lmdb ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(m1 PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.m1_src)
endif()
set_target_properties(m1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS m1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(m1)
endif()
