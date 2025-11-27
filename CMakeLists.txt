#==============================================================================
# nfx-datatypes - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-datatypes
	VERSION 0.1.1
	DESCRIPTION "Cross-platform C++ library with high-precision Int128 and Decimal datatypes"
	HOMEPAGE_URL "https://github.com/nfx-libs/nfx-datatypes"
	LANGUAGES CXX
)

#----------------------------------------------
# Installation prefix configuration
#----------------------------------------------

# Set default installation path to nfx
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	if(WIN32)
		set(CMAKE_INSTALL_PREFIX "C:/Program Files/nfx" CACHE PATH "Installation directory" FORCE)
	else()
		set(CMAKE_INSTALL_PREFIX "/usr/local/nfx" CACHE PATH "Installation directory" FORCE)
	endif()
	message(STATUS "Setting install prefix to '${CMAKE_INSTALL_PREFIX}'")
endif()

#----------------------------------------------
# Version configuration
#----------------------------------------------

set(NFX_DATATYPES_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} CACHE INTERNAL "nfx-datatypes major version")
set(NFX_DATATYPES_VERSION_MINOR ${PROJECT_VERSION_MINOR} CACHE INTERNAL "nfx-datatypes minor version")
set(NFX_DATATYPES_VERSION_PATCH ${PROJECT_VERSION_PATCH} CACHE INTERNAL "nfx-datatypes patch version")
set(NFX_DATATYPES_VERSION       ${PROJECT_VERSION}       CACHE INTERNAL "nfx-datatypes version")

#----------------------------------------------
# Project metadata
#----------------------------------------------

set(NFX_DATATYPES_LICENSE "MIT")
set(NFX_DATATYPES_LICENSE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

#----------------------------------------------
# Testing setup
#----------------------------------------------

enable_testing()

#----------------------------------------------
# Build options
#----------------------------------------------

# --- Library build types ---
option(NFX_DATATYPES_BUILD_STATIC         "Build static library"                OFF )
option(NFX_DATATYPES_BUILD_SHARED         "Build shared library"                OFF )

# --- Build components ---
option(NFX_DATATYPES_BUILD_TESTS          "Build tests"                         OFF )
option(NFX_DATATYPES_BUILD_SAMPLES        "Build samples"                       OFF )
option(NFX_DATATYPES_BUILD_BENCHMARKS     "Build benchmarks"                    OFF )
option(NFX_DATATYPES_BUILD_DOCUMENTATION  "Build Doxygen documentation"         OFF )

# --- Installation ---
option(NFX_DATATYPES_INSTALL_PROJECT      "Install project"                     OFF )

# --- Packaging ---
option(NFX_DATATYPES_PACKAGE_SOURCE       "Enable source package generation"    OFF )
option(NFX_DATATYPES_PACKAGE_ARCHIVE      "Enable TGZ/ZIP package generation"   OFF )
option(NFX_DATATYPES_PACKAGE_DEB          "Enable DEB package generation"       OFF )
option(NFX_DATATYPES_PACKAGE_RPM          "Enable RPM package generation"       OFF )
option(NFX_DATATYPES_PACKAGE_WIX          "Enable WiX Windows installer (MSI)"  OFF )

#----------------------------------------------
# CMake modules configuration
#----------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#----------------------------------------------
# Main configuration
#----------------------------------------------

include(nfxDataTypesBuildConfig)
include(nfxDataTypeDependencies)
include(nfxDataTypeSources)
include(nfxDataTypeTargets)

#----------------------------------------------
# Installation and packaging
#----------------------------------------------

include(nfxDataTypeInstall)
include(nfxDataTypePackaging)

#----------------------------------------------
# Targets
#----------------------------------------------

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmark)
add_subdirectory(doc)
