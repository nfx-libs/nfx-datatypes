#==============================================================================
# nfx-datatypes - CMake build configuration
#==============================================================================

#----------------------------------------------
# Build validation
#----------------------------------------------

# --- Validate CMake version ---
if(CMAKE_VERSION VERSION_LESS "3.20")
	message(FATAL_ERROR "CMake 3.20 or higher is required for reliable C++20 support")
endif()

# --- Prevent in-source builds ---
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# --- Ensure at least one library type is built ---
if(NOT NFX_DATATYPES_BUILD_STATIC AND NOT NFX_DATATYPES_BUILD_SHARED)
	message(WARNING "Neither NFX_DATATYPES_BUILD_STATIC nor NFX_DATATYPES_BUILD_SHARED is enabled.")
	
	if(DEFINED CACHE{NFX_DATATYPES_BUILD_STATIC} AND DEFINED CACHE{NFX_DATATYPES_BUILD_SHARED})
		message(STATUS "Both library types were explicitly disabled.")
		message(STATUS "Applying fallback: Enabling static library build")
		set(NFX_DATATYPES_BUILD_STATIC ON CACHE BOOL "Build static library (fallback)" FORCE)
	else()
		message(STATUS "Defaulting to static library build")
		set(NFX_DATATYPES_BUILD_STATIC ON)
	endif()
endif()

#----------------------------------------------
# Multi-config generator setup
#----------------------------------------------

# --- For multi-config generators, set available configurations ---
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Release;Debug;RelWithDebInfo;MinSizeRel" CACHE STRING "Available build configurations" FORCE)
	message(STATUS "Multi-config generator detected. Available configurations: ${CMAKE_CONFIGURATION_TYPES}")
else()
	# --- For single-config generators (Makefiles, Ninja), set default build type ---
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
		message(STATUS "Single-config generator detected. Defaulting to optimized build type: ${CMAKE_BUILD_TYPE}")
	else()
		message(STATUS "Single-config generator detected. Using specified build type: ${CMAKE_BUILD_TYPE}")
	endif()
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "RelWithDebInfo" "MinSizeRel")
endif()

#----------------------------------------------
# Install prefix configuration
#----------------------------------------------

include(GNUInstallDirs)

#----------------------------------------------
# Directory configuration
#----------------------------------------------

set(NFX_DATATYPES_DIR          "${PROJECT_SOURCE_DIR}"         CACHE PATH  "Root directory"   )
set(NFX_DATATYPES_INCLUDE_DIR  "${NFX_DATATYPES_DIR}/include"  CACHE PATH  "Include directory")
set(NFX_DATATYPES_SOURCE_DIR   "${NFX_DATATYPES_DIR}/src"      CACHE PATH  "Source directory" )

#----------------------------------------------
# Output directory configuration
#----------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
