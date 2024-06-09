cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
project(Client)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp" "vendor/imgui/*.cpp" "vendor/glad/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include)
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mysql-connector/include)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/lib)
# target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mysql-connector/lib)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


target_link_libraries(${PROJECT_NAME} glfw3 wsock32 ws2_32 nlohmann_json::nlohmann_json)