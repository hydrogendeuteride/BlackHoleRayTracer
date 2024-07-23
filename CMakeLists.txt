cmake_minimum_required(VERSION 3.22)
project(BlackHoleRayTracer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Fetch and include GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Common settings
include_directories(glad/include include imgui)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${glm_SOURCE_DIR})

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shader)
set(IMAGE_DIR ${CMAKE_SOURCE_DIR}/image)
set(FONT_DIR ${CMAKE_SOURCE_DIR}/fonts)

set(IMGUI_SRC
        "imgui/imgui.cpp"
        "imgui/imgui_draw.cpp"
        "imgui/imgui_impl_glfw.cpp"
        "imgui/imgui_impl_opengl3.cpp"
        "imgui/imgui_tables.cpp"
        "imgui/imgui_widgets.cpp")

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

# Common executable and linking settings
add_executable(BlackHoleRayTracer
        main.cpp
        src/shader.cpp
        glad/src/glad.c
        src/render.cpp
        src/texture.cpp
        ${IMGUI_SRC}
        src/imguiWidget.cpp
)

target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_DL_LIBS})

set_target_properties(${PROJECT_NAME}
        PROPERTIES SUFFIX ".html"
        LINK_FLAGS " --bind -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -std=c++20 -s ALLOW_MEMORY_GROWTH=1 -s EXCEPTION_CATCHING_ALLOWED=[..] -s USE_WEBGL2=1 -g4"
)

add_custom_command(TARGET BlackHoleRayTracer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BlackHoleRayTracer>/shader"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BlackHoleRayTracer>/image"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BlackHoleRayTracer>/fonts"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} "$<TARGET_FILE_DIR:BlackHoleRayTracer>/shader"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${IMAGE_DIR} "$<TARGET_FILE_DIR:BlackHoleRayTracer>/image"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${FONT_DIR} "$<TARGET_FILE_DIR:BlackHoleRayTracer>/fonts"
)