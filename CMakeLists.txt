cmake_minimum_required(VERSION 3.24)

project(HotdogsRun)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "app/src/*.cpp" "vox/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "app/include" ".")

target_link_libraries(${PROJECT_NAME}
	vulkan
	glfw
)
