cmake_minimum_required(VERSION 3.30)

project(LongDo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 COMPONENTS Quick Gui REQUIRED)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(long-do
  src/main.cpp
)

target_include_directories(long-do
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ioc
)

target_compile_options(long-do
  PRIVATE
  -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Woverloaded-virtual -Wunused -Wcast-align -Wconversion -Wmisleading-indentation -Wdouble-promotion -Wimplicit-fallthrough
  $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},GNU>:-Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wuseless-cast>
  $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},Clang>:-Weverything -Wno-global-constructors -Wno-missing-prototypes -Wno-exit-time-destructors -Wno-c++98-compat -Wno-c++20-compat>
)

set_source_files_properties(qml/main.qml PROPERTIES
    QT_RESOURCE_ALIAS main.qml
)

qt_add_qml_module(long-do
  URI long_do
  VERSION 1.0
  QML_FILES
  qml/main.qml
)

target_link_libraries(long-do PRIVATE Qt6::Gui Qt6::Quick)

if(CLANG_TIDY_RUN)
  file(WRITE "${CMAKE_BINARY_DIR}/.clang-tidy" "Checks: '-*,llvm-twine-local'\n")
endif()

if (ENABLE_UT)
  find_package(Qt6 6.5 REQUIRED COMPONENTS Test)
  enable_testing(true)
  set_target_properties(long-do PROPERTIES EXCLUDE_FROM_ALL ON)
  add_subdirectory(ut)
endif()
