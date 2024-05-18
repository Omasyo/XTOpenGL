cmake_minimum_required(VERSION 3.19)

project(XTOpenGL)


file(GLOB SOURCE *.cpp )

add_executable(build-debug 
  src/main.cpp

  external/glad/src/glad.c
  external/imgui/imgui.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)

target_compile_features(build-debug PRIVATE cxx_std_20)

target_include_directories( build-debug
  PRIVATE external/glad/include
  PRIVATE external/glm
  PRIVATE external/imgui
  PRIVATE external/imgui/backends
  PRIVATE external/stb
)

target_link_libraries(build-debug PRIVATE  -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(TEXTURE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data/textures)
set(TEXTURE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/textures)

add_custom_target(create-textures
  mkdir -p ${SHADER_BINARY_DIR}
  COMMAND cp -u -r ${TEXTURE_SOURCE_DIR} ${TEXTURE_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/textures/
)

add_custom_target(create-shaders
  mkdir -p ${SHADER_BINARY_DIR}
  COMMAND cp -u -r ${SHADER_SOURCE_DIR} ${SHADER_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/shaders/
)
