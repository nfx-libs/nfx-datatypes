#==============================================================================
# nfx-datatypes - CMake Sources
#==============================================================================

#----------------------------------------------
# Conditional headers and sources
#----------------------------------------------

set(PRIVATE_SOURCES)

list(APPEND PRIVATE_SOURCES
	${NFX_DATATYPES_SOURCE_DIR}/Decimal.cpp
	${NFX_DATATYPES_SOURCE_DIR}/Int128.cpp
)
