cmake_minimum_required(VERSION 3.14)

project(m1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(m1
  main.cpp
  m1_env.h
  m1_env.cpp
  m1_store.h
  m1_store.cpp
  m1_lv0_item.h
  m1_lv0_item_members.cpp
  m1_lv0_item_test.cpp
  m1_lv2_item.h
  m1_lv2_item.cpp
  m1_constants.h
  m1_lv1_item.h
  m1_lv1_item.cpp
  m1_lv1_item_test.cpp
  main_test_store_lv1.cpp
)
target_link_libraries(m1 Qt${QT_VERSION_MAJOR}::Core gtest gtest_main lmdb ${Boost_FILESYSTEM_LIBRARY})

add_executable(m1_test_lv0_item
  main_test_store_lv0.cpp
  m1_env.h
  m1_env.cpp
  m1_store.h
  m1_store.cpp
  m1_lv0_item.h
  m1_lv0_item_members.cpp
  m1_lv0_item_test.cpp
  m1_lv2_item.h
  m1_lv2_item.cpp
)
target_link_libraries(m1_test_lv0_item Qt${QT_VERSION_MAJOR}::Core gtest gtest_main lmdb ${Boost_FILESYSTEM_LIBRARY})

add_executable(m1_test_lv1_item
  main_test_store_lv1.cpp
  m1_env.h
  m1_env.cpp
  m1_store.h
  m1_store.cpp
  m1_lv0_item.h
  m1_lv0_item_members.cpp
  m1_lv0_item_test.cpp
  m1_lv1_item.h
  m1_lv1_item.cpp
  m1_lv1_item_test.cpp
  m1_lv2_item.h
  m1_lv2_item.cpp
)
target_link_libraries(m1_test_lv1_item Qt${QT_VERSION_MAJOR}::Core gtest gtest_main lmdb ${Boost_FILESYSTEM_LIBRARY})

include(GNUInstallDirs)
install(TARGETS m1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
