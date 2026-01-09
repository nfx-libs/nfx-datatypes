#==============================================================================
# nfx-datatypes - Library installation
#==============================================================================

#----------------------------------------------
# Installation condition check
#----------------------------------------------

if(NOT NFX_DATATYPES_INSTALL_PROJECT)
    message(STATUS "Installation disabled, skipping...")
    return()
endif()

#----------------------------------------------
# Install headers
#----------------------------------------------

install(
    DIRECTORY "${NFX_DATATYPES_INCLUDE_DIR}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Development
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl"
)

#----------------------------------------------
# Install library targets
#----------------------------------------------

set(install_targets)

if(NFX_DATATYPES_BUILD_SHARED)
    list(APPEND install_targets ${PROJECT_NAME})
endif()

if(NFX_DATATYPES_BUILD_STATIC)
    list(APPEND install_targets ${PROJECT_NAME}-static)
endif()

if(install_targets)
    install(
        TARGETS ${install_targets}
        EXPORT nfx-datatypes-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    # Install PDB files for debug builds on Windows
    if(MSVC AND NFX_DATATYPES_BUILD_SHARED AND TARGET ${PROJECT_NAME})
        install(
            FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            CONFIGURATIONS Debug RelWithDebInfo
            COMPONENT Development
            OPTIONAL
        )
    endif()
endif()

#----------------------------------------------
# Install CMake config files
#----------------------------------------------

install(
    EXPORT nfx-datatypes-targets
    FILE nfx-datatypes-targets.cmake
    NAMESPACE nfx-datatypes::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nfx-datatypes
    COMPONENT Development
)

# Install separate target files for each configuration (multi-config generators)
if(CMAKE_CONFIGURATION_TYPES)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        install(
            EXPORT nfx-datatypes-targets
            FILE nfx-datatypes-targets-${CONFIG}.cmake
            NAMESPACE nfx-datatypes::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nfx-datatypes
            CONFIGURATIONS ${CONFIG}
            COMPONENT Development
        )
    endforeach()
endif()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nfx-datatypes-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nfx-datatypes-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nfx-datatypes-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nfx-datatypes
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/nfx-datatypes-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/nfx-datatypes-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nfx-datatypes
    COMPONENT Development
)

#----------------------------------------------
# Install license files
#----------------------------------------------

install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt"
    DESTINATION "${CMAKE_INSTALL_DOCDIR}/licenses"
    RENAME "LICENSE-${PROJECT_NAME}.txt"
)

file(GLOB license_files "${CMAKE_CURRENT_SOURCE_DIR}/licenses/LICENSE-*")
foreach(license_file ${license_files})
    get_filename_component(license_name ${license_file} NAME)
    install(
        FILES ${license_file}
        DESTINATION "${CMAKE_INSTALL_DOCDIR}/licenses"
        RENAME "${license_name}.txt"
    )
endforeach()

#----------------------------------------------
# Install documentation
#----------------------------------------------

if(NFX_DATATYPES_BUILD_DOCUMENTATION)
    install(
        DIRECTORY "${CMAKE_BINARY_DIR}/doc/html"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        OPTIONAL
        COMPONENT Documentation
    )
    
    if(WIN32)
        # Install Windows .cmd batch file
        install(
            FILES "${CMAKE_BINARY_DIR}/doc/index.html.cmd"
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL
            COMPONENT Documentation
        )
    else()
        # Install Unix symlink
        install(
            FILES "${CMAKE_BINARY_DIR}/doc/index.html"
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL
            COMPONENT Documentation
        )
    endif()
endif()

message(STATUS "Installation configured for targets: ${install_targets}")
