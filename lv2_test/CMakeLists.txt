cmake_minimum_required(VERSION 3.14)

project(lv2_test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(lv2_test
    ../m1A_constants.h
    ../m1A_env.h
    ../m1A_env.cpp
    ../m1B_store.h
    ../m1B_store.cpp
    ../m1B_lv0_item.h
    ../m1B_lv0_item.cpp
    ../m1B_lv1_item.h
    ../m1B_lv1_item.cpp
    ../m1B_lv2_item.h
    ../m1B_lv2_item.cpp
    ../m1B_lv2_item_test.cpp
    ../main_test_store_lv2.cpp
)
target_link_libraries(lv2_test Qt${QT_VERSION_MAJOR}::Core gtest gtest_main lmdb ${Boost_FILESYSTEM_LIBRARY})

include(GNUInstallDirs)
install(TARGETS lv2_test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
