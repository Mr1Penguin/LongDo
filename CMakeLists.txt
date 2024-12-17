cmake_minimum_required(VERSION 3.30)

project(LongDo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 COMPONENTS Quick Gui REQUIRED)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable (long-do
  src/main.cpp
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
  file(WRITE "${CMAKE_BINARY_DIR}/.clang-tidy" "Checks: '-*'\n")
endif()
