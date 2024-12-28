qt_add_executable(ioc_test ut_ioc.cpp)
target_include_directories(ioc_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src/ioc
)

target_link_libraries(ioc_test
  PRIVATE
  Qt::Test
)

add_test(NAME ioc_test COMMAND ioc_test)

qt_add_executable(container_test ut_container.cpp)
target_include_directories(container_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src/ioc
)

target_link_libraries(container_test
  PRIVATE
  Qt::Test
)

add_test(NAME container_test COMMAND container_test)
