cmake_minimum_required(VERSION 2.6)
project(SIMPLEEVENTBUS)

set(SIMPLEEVENTBUS_VERSION "0.1")

option(SIMPLEEVENTBUS_EXAMPLE "Compile example program" ON)
option(SIMPLEEVENTBUS_TEST "Compile tests" ON)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ftree-vectorize -msse3 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG -Werror")
endif()

add_subdirectory(src/example)
add_subdirectory(src/test)

find_program(DOXYGEN "doxygen")
if (DOXYGEN)
	message("doxygen found")
else()
	message("doxygen not found")
endif()

include(FindPkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	set(prefix ${CMAKE_INSTALL_PREFIX})
	set(exec_prefix "\${prefix}")
	set(libdir "\${exec_prefix}/lib${LIB_SUFFIX}")
	set(bindir "\${exec_prefix}/bin")
	set(includedir "\${prefix}/include")
	configure_file(${SIMPLEEVENTBUS_SOURCE_DIR}/libsimpleeventbus.pc.in ${SIMPLEEVENTBUS_BINARY_DIR}/libsimpleeventbus.pc @ONLY)
	install(FILES ${SIMPLEEVENTBUS_BINARY_DIR}/libsimpleeventbus.pc DESTINATION "lib${LIB_SUFFIX}/pkgconfig")
endif()

install(FILES ${SIMPLEEVENTBUS_SOURCE_DIR}/src/include/simpleeventbus/EventBus.h DESTINATION include/simpleeventbus)

# TODO execute doxygen
