cmake_minimum_required(VERSION 3.25)
project(BlackHoleRayTracer)

set(CMAKE_CXX_STANDARD 23)

# Common settings
include_directories(glad/include include)

# Platform-specific settings
if (WIN32)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows/share")
    else()
        message(FATAL_ERROR "Please define VCPKG_ROOT environment variable pointing to your vcpkg installation")
    endif()

    set(glfw3_DIR  "${CMAKE_SOURCE_DIR}/Lib/glfw-build/src")
    find_package(glfw3 CONFIG REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})

else() # Linux settings
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx2 -fopenmp -march=native")

    find_package(glfw3 3.3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    link_libraries(${GLFW_LIBRARY_DIRS})
endif()

# Common executable and linking settings
add_executable(BlackHoleRayTracer
        main.cpp
        src/shader.cpp
        glad/src/glad.c
        src/render.cpp
        src/texture.cpp
)

target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_DL_LIBS})
