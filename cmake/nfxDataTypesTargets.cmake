#==============================================================================
# nfx-datatypes - CMake targets
#==============================================================================

#----------------------------------------------
# Targets definition
#----------------------------------------------

# --- Create shared library if requested ---
if(NFX_DATATYPES_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

# --- Create static library if requested ---
if(NFX_DATATYPES_BUILD_STATIC)
    add_library(${PROJECT_NAME}-static STATIC)
    target_sources(${PROJECT_NAME}-static
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME}-static
        PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}-static
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::static ALIAS ${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Targets properties
#----------------------------------------------

function(configure_target target_name)
    # --- Include directories ---
    target_include_directories(${target_name}
        PUBLIC
            $<BUILD_INTERFACE:${NFX_DATATYPES_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${NFX_DATATYPES_SOURCE_DIR}
    )

    # --- C++20 standard ---
    target_compile_features(${target_name}
        PUBLIC
            cxx_std_20
    )

    # --- Properties ---
    set_target_properties(${target_name}
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            DEBUG_POSTFIX "-d"
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            POSITION_INDEPENDENT_CODE ON
    )

    # --- CPU optimizations (Release/RelWithDebInfo only) ---
    if(NFX_DATATYPES_ENABLE_SIMD)
        target_compile_options(${target_name}
            PRIVATE
                $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:/arch:AVX2>
                $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:-march=native>
        )
    endif()

    # --- Compiler warnings ---
    target_compile_options(${target_name}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Werror>
    )
endfunction()

# --- Apply configuration to both targets ---
if(NFX_DATATYPES_BUILD_SHARED)
    configure_target(${PROJECT_NAME})
    if(WIN32)
        set_target_properties(${PROJECT_NAME}
            PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS TRUE
        )

        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/nfxDataTypesVersion.rc.in
            ${CMAKE_BINARY_DIR}/nfxDataTypes.rc
            @ONLY
        )
        target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/nfxDataTypes.rc)
    endif()
endif()

if(NFX_DATATYPES_BUILD_STATIC)
    configure_target(${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Build configuration summary
#----------------------------------------------

if(NFX_DATATYPES_ENABLE_SIMD)
    message(STATUS "nfx-datatypes: Native CPU optimizations enabled (Release/RelWithDebInfo builds)")
else()
    message(STATUS "nfx-datatypes: Native CPU optimizations disabled (suitable for WebAssembly)")
endif()
