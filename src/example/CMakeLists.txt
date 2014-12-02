if (SIMPLEEVENTBUS_EXAMPLE)
	include_directories(${SIMPLEEVENTBUS_SOURCE_DIR}/src/include/simpleeventbus)
	file(GLOB_RECURSE SRC ${SIMPLEEVENTBUS_SOURCE_DIR}/src/example/*.cpp)
	add_executable(example ${SRC})
endif()

