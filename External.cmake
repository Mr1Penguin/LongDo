if(DOWNLOAD_BOOST_DI)
  set(EXPECTED_BOOST_DI_HASH 96d8d38ff258bfb60cab19ee74b2e5f7)
  if(EXISTS ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp)
    execute_process(COMMAND md5sum ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp
      OUTPUT_VARIABLE BOOST_DI_HASH
    )
    if (NOT BOOST_DI_HASH STREQUAL EXPECTED_BOOST_DI_HASH)
      file(REMOVE ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp)
    endif()
  endif()
  if (NOT EXISTS ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp)
    message(STATUS "Downloading boost-di")

    file(DOWNLOAD
      https://raw.githubusercontent.com/boost-ext/di/cpp14/include/boost/di.hpp
      ${CMAKE_BINARY_DIR}/ext/boost-di/boost/di.hpp
      EXPECTED_MD5 ${EXPECTED_BOOST_DI_HASH}
      STATUS boost-di-status
    )
  
    list(GET boost-di-status 0 status-code)
    if(NOT status-code EQUAL 0)
      message(FATAL_ERROR "Failed to download boost-di")
    endif()
  
    add_library(boost-di INTERFACE)
    target_include_directories(boost-di SYSTEM INTERFACE ${CMAKE_BINARY_DIR}/ext/boost-di)
  endif()
endif()
