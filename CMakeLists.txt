cmake_minimum_required(VERSION 3.0.0)
project(GameofLife VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(/opt/homebrew/include) #change to include for system
link_directories(/opt/homebrew/lib) #change to the lib for your system

include_directories(./glad/include)

add_executable(main main.cpp)

add_subdirectory(glad/)
target_link_libraries(main glad)

target_link_libraries(main glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(main OpenGL::GL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
