cmake_minimum_required(VERSION 3.30)

project(LongDo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

list(APPEND CMAKE_FIND_ROOT_PATH ${PROJECT_SOURCE_DIR}/local_root)

find_path(boost-di-root
  NAMES
  "boost/di.hpp"
  REQUIRED
)

message(STATUS "Found boost-di: ${boost-di-root}")
add_library(boost-di INTERFACE IMPORTED)
target_include_directories(boost-di SYSTEM INTERFACE ${boost-di-root})

find_package(Qt6 6.5 COMPONENTS Quick Gui REQUIRED)
find_package(KF6
  REQUIRED
  COMPONENTS
  Kirigami
  I18n
  CoreAddons
  QQC2DesktopStyle
  IconThemes
)
ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(long-do
  src/main.cpp
  src/MainPageLoader.cpp
)

target_include_directories(long-do
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ioc
)

add_compile_options(
  -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror -Woverloaded-virtual -Wunused -Wcast-align -Wconversion -Wmisleading-indentation -Wdouble-promotion -Wimplicit-fallthrough
  "$<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches$<SEMICOLON>-Wduplicated-cond$<SEMICOLON>-Wlogical-op$<SEMICOLON>-Wuseless-cast$<SEMICOLON>-fconcepts-diagnostics-depth=3>"
  "$<$<CXX_COMPILER_ID:Clang>:-Weverything$<SEMICOLON>-Wno-global-constructors$<SEMICOLON>-Wno-missing-prototypes$<SEMICOLON>-Wno-exit-time-destructors$<SEMICOLON>-Wno-c++98-compat$<SEMICOLON>-Wno-c++20-compat>"
  "$<$<BOOL:${ENABLE_SAN}>:-fsanitize=undefined$<SEMICOLON>-fsanitize=address$<SEMICOLON>-fno-omit-frame-pointer>"
)

add_link_options(
  "$<$<BOOL:${ENABLE_SAN}>:-fsanitize=undefined$<SEMICOLON>-fsanitize=address$<SEMICOLON>-fno-omit-frame-pointer>"
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

target_link_libraries(long-do
  PRIVATE
  Qt6::Gui
  Qt6::Quick
  boost-di
  KF6::I18n
  KF6::CoreAddons
  KF6::IconThemes
)

if(CLANG_TIDY_RUN)
  file(WRITE "${CMAKE_BINARY_DIR}/.clang-tidy" "Checks: '-*,llvm-twine-local'\n")
endif()

if (ENABLE_UT)
  include(CTest)
  find_package(Qt6 6.5 REQUIRED COMPONENTS Test)
  set_target_properties(long-do PROPERTIES EXCLUDE_FROM_ALL ON)
  add_subdirectory(ut)
endif()
