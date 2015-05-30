#Change this if you need to target a specific CMake version

cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0015 NEW)

set(CMAKE_USER_MAKE_RULES_OVERRIDE
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/cxx_flag_overrides.cmake)

project(extLOG)

set(CMAKE_SUPPRESS_REGENERATION 1)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE INTERNAL FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Check for 32bit vs. 64bit system
if (CMAKE_SIZEOF_VOID_P EQUAL 8) #64bit
	message(FATAL_ERROR "Warning: You are on a 64-bit system, make sure you are pulling in the appropriate 32-bit libraries.")	
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4) #32bit
	message(STATUS "You are on a 32-bit system.")
else()
	message(STATUS "CMake wasn't able to identify if you are on 32-bit or 64-bit...")
endif()


SET(SOURCES
	../src/memory_allocator.cpp
	../src/ext.cpp
	../src/main.cpp
	../src/dll.rc
)

SET(COMPILE_EXTENSION TRUE CACHE BOOL "Compile extLog")


set(EXECUTABLE_NAME "extLOG")
add_library (${EXECUTABLE_NAME} SHARED ${SOURCES})


# -----------------------
# START STATIC LIBRARIES
# -----------------------
if(${UNIX})
     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

# OpenSSL Headers
find_package(OpenSSL)
if(OPENSSL_FOUND)
	include_directories("${OPENSSL_INCLUDE_DIR}")
	target_link_libraries(${EXECUTABLE_NAME} ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
else()
	message(FATAL_ERROR "\nOpenSSL not found")
endif()

# BOOST
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
if(${WIN32})
	add_definitions(-D _WIN32_WINNT=0x0600)
endif()
if (COMPILE_EXTENSION)
	find_package(Boost REQUIRED COMPONENTS chrono date_time filesystem random regex system thread)
else()
	find_package(Boost REQUIRED COMPONENTS chrono date_time filesystem program_options random regex system thread)
endif()
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "\nBOOST not found\n Please Set BOOST_ROOT & BOOST_LIBRARYDIR")
endif()

# POCO
if(${WIN32})
	add_definitions(-D POCO_NO_UNWINDOWS)
endif()
include(FindPOCO)
if(POCO_FOUND)
	include_directories(${POCO_INCLUDE_DIR} ${POCO_Crypto_INCLUDE_DIR} ${POCO_Data_INCLUDE_DIR})
	include_directories(${POCO_Data_SQLite_INCLUDE_DIR} ${POCO_Data_MYSQL_INCLUDE_DIR} ${POCO_Net_INCLUDE_DIR} ${POCO_Util_INCLUDE_DIR})
else()
	message(FATAL_ERROR "\nPOCO not found\n Please Set POCO_ROOT")
endif()

# MySQL
include(FindMySQL)
if(MYSQL_FOUND)
	include_directories(${MYSQL_INCLUDE_DIR})
else()
	message(FATAL_ERROR "\nMySQL not found")
endif()

if(${UNIX})
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
endif()
# -----------------------
# END STATIC LIBRARIES
# -----------------------


# -----------------------
# START DYNAMIC LIBRARIES
# -----------------------

# Look for Intel Threading Building Blocks (TBB)
include(FindTBB)
if(TBB_FOUND)
	include_directories(${TBB_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "\nIntel TBB not found\n Please Set TBB_ROOT")
endif()

# -----------------------
# END DYNAMIC LIBRARIES
# -----------------------

if(${WIN32})
	# Windows
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES}  ${POCO_LIBRARIES} ${MYSQL_LIBRARY} ${TBB_MALLOC_LIBRARY} Iphlpapi)
	add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)
	SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS " /MANIFEST:NO /ERRORREPORT:NONE")
else()
endif()

mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)



message(STATUS "C_FLAGS:  =${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG:=${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE:=${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS_MINSIZEREL:=${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO:=${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "")
message(STATUS "")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE:=${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:=${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
