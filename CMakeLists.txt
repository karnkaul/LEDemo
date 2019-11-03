cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(LEDemo C CXX)

################################################################################
# Init
################################################################################
# Init/Update Submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/../.git")
	option(UPDATE_SUBMODULES "Init/Update submodules during generation" ON)
	if(UPDATE_SUBMODULES)
		message(STATUS "Updating git submodules...")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
			RESULT_VARIABLE UPDATE_SUBMODULES_RESULT
		)
		if(NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update failed!")
		endif()
	endif()
endif()

get_filename_component(ROOT_PATH "${CMAKE_SOURCE_DIR}/.." ABSOLUTE)
if(CI_BUILD)
	set(BUILD_PATH "${ROOT_PATH}/_Build/CI")
else()
	set(BUILD_PATH "${ROOT_PATH}/_Build")
endif()
set(EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/Runtime")
set(LIBRARIES_PATH "${EXECUTABLE_PATH}/Lib")

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
