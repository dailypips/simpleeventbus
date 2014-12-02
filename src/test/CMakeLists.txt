if (SIMPLEEVENTBUS_TEST)
	include_directories(${SIMPLEEVENTBUS_SOURCE_DIR}/src/include/simpleeventbus ${SIMPLEEVENTBUS_SOURCE_DIR}/src/test/gtest ${SIMPLEEVENTBUS_SOURCE_DIR}/src/test/gtest/include)
	file(GLOB_RECURSE SRC ${SIMPLEEVENTBUS_SOURCE_DIR}/src/test/*.cc ${SIMPLEEVENTBUS_SOURCE_DIR}/src/test/*.cpp ${SIMPLEEVENTBUS_SOURCE_DIR}/src/test/*.h)
	add_executable(tests ${SRC})

	if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
		target_link_libraries(tests pthread)
	endif()
endif()

