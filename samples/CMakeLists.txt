#==============================================================================
# nfx-datatypes - Samples
#==============================================================================

#----------------------------------------------
# Sample condition check
#----------------------------------------------

if(NOT NFX_DATATYPES_BUILD_SAMPLES)
	message(STATUS "Samples disabled, skipping...")
	return()
endif()

#----------------------------------------------
# Samples source files
#----------------------------------------------

set(SAMPLE_SOURCES)

list(APPEND SAMPLE_SOURCES
	Sample_Datatypes.cpp
)

#----------------------------------------------
# Configure samples executables
#----------------------------------------------

foreach(sample_source ${SAMPLE_SOURCES})
	get_filename_component(sample_target_name ${sample_source} NAME_WE)

	if(NOT TARGET ${sample_target_name})
		add_executable(${sample_target_name} ${sample_source})

		#----------------------------------------------
		# Target linking
		#----------------------------------------------

		target_link_libraries(${sample_target_name} PRIVATE
			nfx-datatypes::static
		)

		#----------------------------------------------
		# Enable specific CPU features
		#----------------------------------------------

		target_compile_options(${sample_target_name} PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
			$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.2>
		)

		#----------------------------------------------
		# Properties
		#----------------------------------------------

		set_target_properties(${sample_target_name} PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
			POSITION_INDEPENDENT_CODE ON
			DEBUG_POSTFIX "-d"
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/samples"
			RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/samples"
			RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/samples"
		)
	endif()
endforeach()
