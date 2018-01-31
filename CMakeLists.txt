project(graphics_asgn1)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package(OpenGL REQUIRED)



# Compile external dependencies 
add_subdirectory (external)


include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)



target_link_libraries(graphics_asgn1
	${ALL_LIBS}
)



# Xcode and Visual working directories
set_target_properties(graphics_asgn1 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")


configure_file(src/Sample_GL.vert Sample_GL.vert COPYONLY)
configure_file(src/Sample_GL.frag Sample_GL.frag COPYONLY)
# configure_file(assets/song.mp3 assets/song.mp3 COPYONLY)
