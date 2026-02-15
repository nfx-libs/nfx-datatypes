#==============================================================================
# nfx-datatypes - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-datatypes
    VERSION 0.4.1
    DESCRIPTION "Cross-platform C++ library with high-precision Int128 and Decimal datatypes"
    HOMEPAGE_URL "https://github.com/nfx-libs/nfx-datatypes"
    LANGUAGES CXX
)

#----------------------------------------------
# Version configuration
#----------------------------------------------

set(NFX_DATATYPES_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} CACHE INTERNAL "nfx-datatypes major version")
set(NFX_DATATYPES_VERSION_MINOR ${PROJECT_VERSION_MINOR} CACHE INTERNAL "nfx-datatypes minor version")
set(NFX_DATATYPES_VERSION_PATCH ${PROJECT_VERSION_PATCH} CACHE INTERNAL "nfx-datatypes patch version")
set(NFX_DATATYPES_VERSION       ${PROJECT_VERSION}       CACHE INTERNAL "nfx-datatypes version")

#----------------------------------------------
# Testing setup
#----------------------------------------------

enable_testing()

#----------------------------------------------
# Build options
#----------------------------------------------

# --- Library build types ---
option(NFX_DATATYPES_BUILD_STATIC        "Build static library"             ON )
option(NFX_DATATYPES_BUILD_SHARED        "Build shared library"             OFF)

# --- Build components ---
option(NFX_DATATYPES_BUILD_TESTS         "Build tests"                      OFF)
option(NFX_DATATYPES_BUILD_SAMPLES       "Build samples"                    OFF)
option(NFX_DATATYPES_BUILD_BENCHMARKS    "Build benchmarks"                 OFF)
option(NFX_DATATYPES_BUILD_DOCUMENTATION "Build Doxygen documentation"      OFF)

# --- Performance options ---
option(NFX_DATATYPES_ENABLE_NATIVE_OPTS  "Enable native CPU optimizations"  ON )

# --- Installation and packaging ---
option(NFX_DATATYPES_INSTALL_PROJECT     "Install project"                  OFF)
option(NFX_DATATYPES_PACKAGE_SOURCE      "Enable source package generation" OFF)

#----------------------------------------------
# CMake modules configuration
#----------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#----------------------------------------------
# Main configuration
#----------------------------------------------

include(nfxDataTypesBuildConfig)
include(nfxDataTypesSources)
include(nfxDataTypesTargets)

#----------------------------------------------
# Installation and packaging
#----------------------------------------------

include(nfxDataTypesInstall)
include(nfxDataTypesPackaging)

#----------------------------------------------
# Targets
#----------------------------------------------

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmark)
add_subdirectory(doc)
