if(DOWNLOAD_BOOST_DI)
  message(STATUS "Downloading boost-di")

  file(DOWNLOAD
    https://raw.githubusercontent.com/boost-ext/di/cpp14/include/boost/di.hpp
    ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp
    EXPECTED_MD5 96d8d38ff258bfb60cab19ee74b2e5f7
    STATUS boost-di-status
  )
  
  list(GET boost-di-status 0 status-code)
  if(NOT status-code EQUAL 0)
    message(FATAL_ERROR "Failed to download boost-di")
  endif()
  
  add_library(boost-di INTERFACE)
  target_include_directories(boost-di SYSTEM INTERFACE ${CMAKE_BINARY_DIR}/ext/boost-di)
endif()
