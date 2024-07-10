cmake_minimum_required(VERSION 3.22)
project("Mori")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(lib)

add_subdirectory(thirdparty/enet)
add_subdirectory(thirdparty/json)
add_subdirectory(thirdparty/magic_enum)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/cpr)

file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/lib/**/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_SOURCE_DIR}/lib/**/*.hpp")

add_executable(Mori main.cpp ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(Mori enet nlohmann_json::nlohmann_json magic_enum::magic_enum spdlog::spdlog cpr::cpr)