if(MSVC)
	project(LEDemo CXX RC)
else()
	project(LEDemo CXX)
endif()

################################################################################
# Source groups
################################################################################
## Code
set(SOURCES
	Main.cpp
	
	Gameplay/GameInit.cpp
	Gameplay/GameInit.h

	Gameplay/Entities/Player.h
	Gameplay/Entities/Player.cpp

	Gameplay/Components/ControllerComponent.h
	Gameplay/Components/ControllerComponent.cpp

	Gameplay/UI/UIOptions.h
	Gameplay/UI/UIOptions.cpp

	Gameplay/Worlds/BootWorld.cpp
	Gameplay/Worlds/BootWorld.h
	Gameplay/Worlds/TestWorld.cpp
	Gameplay/Worlds/TestWorld.h
	Gameplay/Worlds.h
)

## Platform Resources
set(GENERATED_SOURCES "")
set(GENERATED_RESOURCES "")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND GENERATED_SOURCES
		Resources/Win64/App.rc
	)
	list(APPEND GENERATED_RESOURCES
		Resources/Win64/Icon.ico
	)
endif()
set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

## Generated
list(APPEND GENERATED_SOURCES
	Resources/Linux/.desktop
	Resources/OSX/Info.plist
)
copy_files_src_to_bin("${GENERATED_RESOURCES}")
configure_files_src_to_bin("${GENERATED_SOURCES}")
set(GENERATED_FILES ${GENERATED_SOURCES} ${GENERATED_RESOURCES})

################################################################################
# Target
################################################################################
list(TRANSFORM GENERATED_FILES PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
source_group(App "${CMAKE_CURRENT_BINARY_DIR}" FILES ${GENERATED_FILES})
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
set(ALL_FILES ${SOURCES} ${GENERATED_FILES})
set(EXE_NAME LEDemo$<$<NOT:$<CONFIG:RelWithDebInfo>>:-$<CONFIG>>${EXE_SUFFIX})
add_le_executable(${EXE_NAME} "${ALL_FILES}")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../Runtime")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
	Engine
)

################################################################################
# Libraries
################################################################################
target_link_libraries(${PROJECT_NAME} PUBLIC
	Engine
)

################################################################################
# Install
# ################################################################################
# install(TARGETS ${PROJECT_NAME}
# 	CONFIGURATIONS ${INSTALL_CONFIGS}
# 	RUNTIME DESTINATION "${RUNTIME_PATH}"
# )
# install(FILES
# 	"${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.png"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/Resources/OSX/Icon.icns"
# 	"${CMAKE_CURRENT_BINARY_DIR}/Resources/OSX/Info.plist"
# 	"${CMAKE_CURRENT_BINARY_DIR}/Resources/Linux/.desktop"
# 	DESTINATION "${RUNTIME_PATH}/Resources"
# )
# install_runtime(${EXE_NAME})
