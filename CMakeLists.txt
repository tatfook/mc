# Author: LiPeng, LiXizhi
# Company: ParaEngine.com
# Date: 2015.11
# Desc: minecraft real time file importer
# Note: 

#set(ISOLATED_BUILD TRUE)

set(NPL_RUNTIME_ROOT ${PROJECT_SOURCE_DIR})
if (ISOLATED_BUILD) 
    cmake_minimum_required(VERSION 3.14.5)

    set(NPL_RUNTIME_ROOT "D:/workspace/npl/WinNPLRuntime/NPLRuntime")
    set(BOOST_ROOT ${NPL_RUNTIME_ROOT}/externals/boost/prebuild/src/boost_1_73_0)
else()
    include(${PROJECT_SOURCE_DIR}/cmake/ucm.cmake)
    include(${PROJECT_SOURCE_DIR}/cmake/ConfigBoost.cmake)

    #message(STATUS "BOOST_INCLUDE_DIR:" ${Boost_INCLUDE_DIR})
    #message(STATUS "BOOST_LIBS:" ${Boost_LIBRARIES} )
endif ()

project (MCImporter)

SET(MCImporter_SRCS
	MCImporter.h
	MCImporter.cpp
	MCblock.h
	MCblock.cpp
	endian.h
	util.h
	util.cpp
	stdafx.h
	stdafx.cpp
	MCImporter.def
	readme.txt
)

##############################  
# web files
file (GLOB MC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    mc/*.cpp
	mc/*.h
    )
SOURCE_GROUP("mc" FILES ${MC_FILES})    
list(APPEND MCImporter_SRCS ${MC_FILES})

# add the binary tree to the search path for include files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${NPL_RUNTIME_ROOT}/ParaEngine/Core")
include_directories("${NPL_RUNTIME_ROOT}/ParaEngine/Framework/Interface")
include_directories("${BOOST_ROOT}")

IF(MSVC)
	# statically link MSVC to reduce dependancies
	foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	 	endif(${flag_var} MATCHES "/MD")
	 	if(${flag_var} MATCHES "/MDd")
	 		string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MDd")
	endforeach(flag_var)
	
	# /GR- Remove RTTI to miminize the executable size
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GR-")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
ENDIF()


IF(MSVC)
	# generate program database (PDB symbol files even in release build)
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
ENDIF(MSVC)

SET(CMAKE_DEBUG_POSTFIX "_d")

# add the executable
if (ISOLATED_BUILD)
	add_executable(MCImporter main.cpp ${MCImporter_SRCS})
else () 
	add_library(MCImporter SHARED ${MCImporter_SRCS})
endif()

if (ISOLATED_BUILD) 
	list(APPEND MCImporter_LIBS Ws2_32.lib)
	list(APPEND MCImporter_LIBS ${BOOST_ROOT}/stage/lib/libboost_filesystem-vc142-mt-sgd-x64-1_73.lib)
	list(APPEND MCImporter_LIBS ${BOOST_ROOT}/stage/lib/libboost_zlib-vc142-mt-sgd-x64-1_73.lib)
	list(APPEND MCImporter_LIBS ${BOOST_ROOT}/stage/lib/libboost_iostreams-vc142-mt-sgd-x64-1_73.lib)
	target_link_libraries(MCImporter ${MCImporter_LIBS})
else() 
	target_link_libraries(MCImporter Ws2_32.lib ${Boost_LIBRARIES})
endif() 



IF(WIN32)
	ADD_DEFINITIONS(-DWNT)
ENDIF(WIN32)

if(MSVC AND NOT ISOLATED_BUILD)
    ADD_CUSTOM_COMMAND(
       TARGET MCImporter
       POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_BIN_DIR}
       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MCImporter> ${OUTPUT_BIN_DIR}
    )
endif()
