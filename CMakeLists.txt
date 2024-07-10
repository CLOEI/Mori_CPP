cmake_minimum_required(VERSION 3.20)
project("Mori")

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)
add_subdirectory(thirdparty/enet)
add_subdirectory(thirdparty/json)
add_subdirectory(thirdparty/magic_enum)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/cpr)

add_executable(Mori main.cpp)
link_libraries(Mori PRIVATE enet json magic_enum::magic_enum spdlog::spdlog cpr::cpr)