qt_add_executable(boost-di.FailCreateNotBinded FailCreateNotBinded.cpp)
target_link_libraries(boost-di.FailCreateNotBinded PRIVATE boost-di)
set_target_properties(boost-di.FailCreateNotBinded PROPERTIES
  EXCLUDE_FROM_ALL TRUE
  EXCLUDE_FROM_DEFAULT_BUILD TRUE
)

add_test(
  NAME boost-di.FailCreateNotBinded
  COMMAND 
  ${CMAKE_COMMAND} --build . --target boost-di.FailCreateNotBinded --config $<CONFIGURATION>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

message (STATUS "COMP: ${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(REGEX "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>\\(\\)'?")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(REGEX "'create<std::shared_ptr<\\(anonymous namespace\\)::Interface>, 0>' has been explicitly marked deprecated here")
else()
  message(FATAL_ERROR "Not maching compiler")
endif()

set_tests_properties(boost-di.FailCreateNotBinded
  PROPERTIES
  PASS_REGULAR_EXPRESSION ${REGEX}
  LABELS noMemcheck
)
