cmake_minimum_required(VERSION 3.10)

project(HelloWorld)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp" "vendor/imgui/*.cpp" "vendor/backends/imgui_impl_glfw.cpp" "vendor/backends/imgui_impl_opengl3.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLAD/include)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/lib)

target_link_libraries(${PROJECT_NAME} glfw3)