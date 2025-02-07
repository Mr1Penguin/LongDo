qt_add_executable(ioc_test ut_ioc.cpp)
target_include_directories(ioc_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src/ioc
)

target_link_libraries(ioc_test
  PRIVATE
  Qt::Test
  boost-di
)

add_test(NAME ioc_test COMMAND ioc_test)
