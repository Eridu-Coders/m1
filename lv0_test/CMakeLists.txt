cmake_minimum_required(VERSION 3.14)

project(lv0_test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(lv0_test
  ../m1_constants.h
  ../m1_lv2_item.cpp
  ../m1_lv2_item.h
  ../m1_store.cpp
  ../m1_lv0_item_members.cpp
  ../m1_lv0_item.h
  ../m1_store.h
  ../m1_lv0_item_test.cpp
  ../m1_env.h
  ../m1_env.cpp
  ../main_test_store_lv0.cpp
)
target_link_libraries(lv0_test Qt${QT_VERSION_MAJOR}::Core gtest gtest_main lmdb ${Boost_FILESYSTEM_LIBRARY})

include(GNUInstallDirs)
install(TARGETS lv0_test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
