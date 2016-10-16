cmake_minimum_required (VERSION 3.6)
project (GP2BaseCode)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(FBX REQUIRED)

add_executable(GP2BaseCode 
	src/main.cpp src/Graphics.cpp src/Input.cpp 
	src/Shader.cpp src/Camera.cpp src/GameObject.cpp
	src/Renderer.cpp src/ShaderProgram.cpp
	src/Model.cpp src/Texture.cpp src/Game.cpp
	src/BaseComponent.cpp src/CameraBehaviour.cpp src/Font.cpp
	src/PostProcessing.cpp src/TerrainComp.cpp src/Scene.cpp src/SceneManager.cpp 
	src/tinyxml2.cpp src/ResourceManager.cpp src/DefRenderer.cpp src/Skybox.cpp
	src/TimeDay.cpp src/MoveGameObjectBehaviour.cpp src/Light.cpp src/WaterComp.cpp
	include/Common.h include/Graphics.h include/Input.h 
	include/Shader.h include/Camera.h include/GameObject.h
	include/Renderer.h include/ShaderProgram.h
	include/Model.h include/Texture.h include/Vertex.h
	include/Game.h include/CameraBehaviour.h include/BaseComponent.h
	include/Font.h include/PostProcessing.h include/TerrainComp.h include/Scene.h
	include/SceneManager.h include/tinyxml2.h include/ResourceManager.h
	include/DefRenderer.h include/Skybox.h include/TimeDay.h include/MoveGameObjectBehaviour.h
	include/Light.h include/WaterComp.h)

include_directories(GP2BaseCode include/ ${OPENGL_INCLUDE_DIRS} 
		${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS}
		${SDL2IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${FBX_INCLUDE_DIR})
target_link_libraries(GP2BaseCode ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} 
		${GLEW_LIBRARIES} ${SDL2IMAGE_LIBRARY} ${SDL2_TTF_LIBRARIES}
		${FBX_LIBRARY})

if (MSVC)
add_custom_command(TARGET GP2BaseCode POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/dlls $<TARGET_FILE_DIR:GP2BaseCode>)
endif (MSVC)

add_custom_command(TARGET GP2BaseCode PRE_BUILD COMMAND ${CMAKE_COMMAND}
					-E copy_directory ${CMAKE_SOURCE_DIR}/assets
					$<TARGET_FILE_DIR:GP2BaseCode>/assets)