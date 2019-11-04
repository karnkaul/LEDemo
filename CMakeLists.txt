cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

project(LEDemo C CXX)
set(CMAKE_DISABLE_SOURCE_CHANGES TRUE)
set(CMAKE_DISABLE_IN_SOURCE_BUILD TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Init
################################################################################
# Init/Update Submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
	option(UPDATE_SUBMODULES "Init/Update submodules during generation" ON)
	if(UPDATE_SUBMODULES AND NOT SUBMODULES_UPDATED)
		message(STATUS "Updating git submodules...")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
			RESULT_VARIABLE UPDATE_SUBMODULES_RESULT
		)
		if(NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update failed!")
		else()
			set(SUBMODULES_UPDATED 1)
		endif()
	endif()
endif()

SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CMAKE_INSTALL_RPATH_USE_ORIGIN TRUE)
	set(CMAKE_INSTALL_RPATH "$ORIGIN/Lib")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(CMAKE_INSTALL_NAME_DIR @executable_path)
	set(CMAKE_INSTALL_RPATH "@executable_path/Lib")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

set(EXECUTABLE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Runtime")
set(LIBRARIES_PATH "${EXECUTABLE_PATH}/Lib")
set(BUILD_LE_TEST 0 CACHE STRING "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_PATH} CACHE STRING "" FORCE)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_PATH} CACHE STRING "" FORCE)
endforeach()

################################################################################
# Sub-projects
################################################################################
add_subdirectory(LittleEngine)
add_subdirectory(LEDemo)
