
# 是否编译成独立的APP
#set(ISOLATED_BUILD TRUE)
# 是否使用NPL_RUNTIME的DEV分支
set(NPL_RUNTIME_DEV TRUE)
# NPL_RUNTIME的源码根目录 dev分支: Client/trunk/ParaEngineClient   cp_old分支: NPLRuntime
project (MCImporter)

set(NPL_RUNTIME_ROOT ${PROJECT_SOURCE_DIR})

if (ISOLATED_BUILD) 
    cmake_minimum_required(VERSION 3.14.5)
    set(NPL_RUNTIME_ROOT "D:/workspace/npl/NPLRuntime_cp_old/NPLRuntime")
else()
endif ()

if (NPL_RUNTIME_DEV)
	set(NPL_RUNTIME_ROOT "D:/workspace/npl/NPLRuntime_win32/Client/trunk/ParaEngineClient")
else() 
	set(NPL_RUNTIME_ROOT "D:/workspace/npl/NPLRuntime_cp_old/NPLRuntime")
endif()

# boost 根目录
# set(BOOST_ROOT D:/ProgramData/Lib/boost_1_78_0)
set(BOOST_ROOT "$ENV{BOOST_ROOT}")
# boost 库目录
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS   ON)
# set(Boost_DEBUG ON)
set(Boost_USE_STATIC_RUNTIME ON)

message(${BOOST_ROOT})
# Add more boost components here
#.\b2 address-model=64 runtime-link=static threading=multi variant=release -sZLIB_SOURCE="D:\workspace\npl\NPLRuntime_win32\Server\trunk\zlib-1.2.3" -sZLIB_INCLUDE="D:\workspace\npl\NPLRuntime_win32\Server\trunk\zlib-1.2.3"
find_package(Boost 1.78 REQUIRED COMPONENTS filesystem iostreams zlib) 


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
if (NPL_RUNTIME_DEV)
	include_directories("${NPL_RUNTIME_ROOT}/Core")
else ()
	# cp_old
	include_directories("${NPL_RUNTIME_ROOT}/ParaEngine/Core")
	include_directories("${NPL_RUNTIME_ROOT}/ParaEngine/Framework/Interface")
endif()
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

target_link_libraries(MCImporter Ws2_32.lib ${Boost_LIBRARIES})

IF(WIN32)
	ADD_DEFINITIONS(-DWNT)
ENDIF(WIN32)


# if(MSVC AND NOT ISOLATED_BUILD)
#     ADD_CUSTOM_COMMAND(
#        TARGET MCImporter
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_BIN_DIR}
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MCImporter> ${OUTPUT_BIN_DIR}
#     )
# endif()
