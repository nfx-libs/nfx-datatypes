#==============================================================================
# nfx-datatypes - Test suite CMake configuration
#==============================================================================

#----------------------------------------------
# Test condition check
#----------------------------------------------

if(NOT NFX_DATATYPES_BUILD_TESTS)
	message(STATUS "Tests disabled, skipping...")
	return()
endif()

#----------------------------------------------
# GoogleTest integration
#----------------------------------------------

include(GoogleTest)

#----------------------------------------------
# Parallel test configuration
#----------------------------------------------

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
	set(CTEST_PARALLEL_LEVEL ${N})
	message(STATUS "Setting parallel test level to ${N} cores")
else()
	set(CTEST_PARALLEL_LEVEL 4)
	message(STATUS "Could not detect CPU count, setting parallel test level to 4")
endif()

#----------------------------------------------
# Tests source files
#----------------------------------------------

set(test_sources)

list(APPEND test_sources
	TESTS_Decimal.cpp
	TESTS_Int128.cpp
)

#----------------------------------------------
# Configure test executables
#----------------------------------------------

foreach(test_source ${test_sources})
	get_filename_component(test_target_name ${test_source} NAME_WE)

	if(NOT TARGET ${test_target_name})
		add_executable(${test_target_name} ${test_source})

		#----------------------------------------------
		# Target linking
		#----------------------------------------------

		target_link_libraries(${test_target_name} PRIVATE
			nfx-datatypes::static
			GTest::gtest_main
		)

		#----------------------------------------------
		# Properties
		#----------------------------------------------

		set_target_properties(${test_target_name} PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
			POSITION_INDEPENDENT_CODE ON
			DEBUG_POSTFIX "-d"
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
			RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
			RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
		)

		#----------------------------------------------
		# Test discovery and registration
		#----------------------------------------------

		gtest_discover_tests(${test_target_name}
			WORKING_DIRECTORY "$<TARGET_FILE_DIR:${test_target_name}>"
			DISCOVERY_MODE POST_BUILD
			PROPERTIES
				TIMEOUT 120
				RUN_SERIAL OFF
		)
	endif()
endforeach()

