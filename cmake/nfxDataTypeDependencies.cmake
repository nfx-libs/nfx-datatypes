#==============================================================================
# nfx-datatypes - Dependencies configuration
#==============================================================================

#----------------------------------------------
# Output configuration
#----------------------------------------------

set(_SAVED_CMAKE_REQUIRED_QUIET     ${CMAKE_REQUIRED_QUIET})
set(_SAVED_CMAKE_MESSAGE_LOG_LEVEL  ${CMAKE_MESSAGE_LOG_LEVEL})
set(_SAVED_CMAKE_FIND_QUIETLY       ${CMAKE_FIND_QUIETLY})

set(CMAKE_REQUIRED_QUIET    ON         )
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE    ) # [ERROR, WARNING, NOTICE, STATUS, VERBOSE, DEBUG]
set(CMAKE_FIND_QUIETLY      ON         )

#----------------------------------------------
# FetchContent dependencies
#----------------------------------------------

include(FetchContent)

if(DEFINED ENV{CI})
	set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)
else()
	set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
endif()
set(FETCHCONTENT_QUIET OFF)

# --- Google test ---
if(NFX_DATATYPES_BUILD_TESTS)
	find_package(GTest QUIET)
	
	if(NOT GTest_FOUND)
		message(STATUS "GoogleTest not found on system, using FetchContent")
		
		set(BUILD_GMOCK         OFF  CACHE BOOL  "Build GoogleMock library"              FORCE)
		set(INSTALL_GTEST       OFF  CACHE BOOL  "Install GoogleTest targets"            FORCE)
		set(GTEST_HAS_ABSL      OFF  CACHE BOOL  "Disable Abseil support in GoogleTest"  FORCE)

		FetchContent_Declare(
			googleTest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG        v1.17.0
			GIT_SHALLOW    TRUE
		)
	else()
		message(STATUS "Using system-installed GoogleTest version ${GTest_VERSION} at ${GTest_DIR}")
	endif()
endif()

# --- Google benchmark ---
if(NFX_DATATYPES_BUILD_BENCHMARKS)
	find_package(benchmark QUIET)
	
	if(NOT benchmark_FOUND)
		message(STATUS "Google Benchmark not found on system, using FetchContent")
		
		set(BENCHMARK_ENABLE_TESTING         OFF  CACHE BOOL  "Build benchmark tests"                          FORCE)
		set(BENCHMARK_ENABLE_EXCEPTIONS      ON   CACHE BOOL  "Enable exceptions in benchmark library"         FORCE)
		set(BENCHMARK_ENABLE_LTO             OFF  CACHE BOOL  "Enable link time optimization"                  FORCE)
		set(BENCHMARK_USE_LIBCXX             OFF  CACHE BOOL  "Use libc++ as the standard library"             FORCE)
		set(BENCHMARK_ENABLE_WERROR          OFF  CACHE BOOL  "Treat warnings as errors"                       FORCE)
		set(BENCHMARK_FORCE_WERROR           OFF  CACHE BOOL  "Force warnings as errors regardless of issues"  FORCE)
		set(BENCHMARK_BUILD_32_BITS          OFF  CACHE BOOL  "Build a 32-bit version of the library"          FORCE)
		set(BENCHMARK_ENABLE_INSTALL         OFF  CACHE BOOL  "Install benchmark targets"                      FORCE)
		set(BENCHMARK_ENABLE_DOXYGEN         OFF  CACHE BOOL  "Build documentation with Doxygen"               FORCE)
		set(BENCHMARK_INSTALL_DOCS           OFF  CACHE BOOL  "Install documentation"                          FORCE)
		set(BENCHMARK_DOWNLOAD_DEPENDENCIES  OFF  CACHE BOOL  "Download and build unmet dependencies"          FORCE)
		set(BENCHMARK_ENABLE_GTEST_TESTS     OFF  CACHE BOOL  "Build benchmark GTest-based tests"              FORCE)
		set(BENCHMARK_USE_BUNDLED_GTEST      OFF  CACHE BOOL  "Use bundled GoogleTest for benchmark"           FORCE)
		set(BENCHMARK_ENABLE_LIBPFM          OFF  CACHE BOOL  "Enable performance counters via libpfm"         FORCE)
		set(ENABLE_ASSEMBLY_TESTS_DEFAULT    OFF  CACHE BOOL  "Enable assembly tests by default"               FORCE)

		FetchContent_Declare(
			googleBenchmark
			GIT_REPOSITORY https://github.com/google/benchmark.git
			GIT_TAG        v1.9.4
			GIT_SHALLOW    TRUE
		)
	else()
		message(STATUS "Using system-installed Google Benchmark version ${benchmark_VERSION} at ${benchmark_DIR}")
	endif()
endif()

#----------------------------------------------
# Dependency fetching
#----------------------------------------------

if(NFX_DATATYPES_BUILD_TESTS)
	if(NOT GTest_FOUND)
		FetchContent_MakeAvailable(googleTest)
	endif()
endif()

if(NFX_DATATYPES_BUILD_BENCHMARKS)
	if(NOT benchmark_FOUND)
		FetchContent_MakeAvailable(googleBenchmark)
	endif()
endif()

#----------------------------------------------
# Cleanup
#----------------------------------------------

set(CMAKE_REQUIRED_QUIET ${_SAVED_CMAKE_REQUIRED_QUIET})
set(CMAKE_MESSAGE_LOG_LEVEL ${_SAVED_CMAKE_MESSAGE_LOG_LEVEL})
set(CMAKE_FIND_QUIETLY ${_SAVED_CMAKE_FIND_QUIETLY})
