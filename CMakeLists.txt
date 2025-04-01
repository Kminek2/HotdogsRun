cmake_minimum_required(VERSION 3.24)

project(Hotdogs)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenAL REQUIRED)

file(GLOB_RECURSE SOURCES "app/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "app/include" "." "lib-include")

target_link_libraries(${PROJECT_NAME}
	Vulkan::Vulkan
	glfw
	OpenAL::OpenAL
)
