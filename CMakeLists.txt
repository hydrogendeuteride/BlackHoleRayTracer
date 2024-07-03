cmake_minimum_required(VERSION 3.25)
project(BlackHoleRayTracer)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx2 -fopenmp -march=native")

find_package(glfw3 3.3 REQUIRED)
find_package(Eigen3 3.3 NO_MODULE)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

include_directories(glad/include include)

add_executable(BlackHoleRayTracer
        main.cpp
        src/shader.cpp
        glad/src/glad.c
        src/render.cpp
)

target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_DL_LIBS})