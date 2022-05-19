cmake_minimum_required(VERSION 3.10)

project(sdl-imgui)

# for filesystem functionality from C++17
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if (WIN32)
    set(WHERE-IS-SDL "${CMAKE_SOURCE_DIR}/include/sdl/lib")
    set(WHERE-IS-SDL-HEADERS "${CMAKE_SOURCE_DIR}/include/sdl/include")
    set(WHERE-IS-GLAD "${CMAKE_SOURCE_DIR}/include/glad/src/glad.c")
    set(WHERE-IS-GLAD-HEADERS "${CMAKE_SOURCE_DIR}/include/glad/include")
endif()

if (UNIX)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

#message(${WHERE-IS-SDL})
#message(${WHERE-IS-SDL-HEADERS})
#message(${WHERE-IS-GLAD})
#message(${WHERE-IS-GLAD-HEADERS})

if (WIN32)
    find_library(SDL2 PATHS ${WHERE-IS-SDL})
    find_library(SDL2main PATHS ${WHERE-IS-SDL})
    include_directories("${WHERE-IS-SDL-HEADERS}")
endif()

add_library("glad" ${WHERE-IS-GLAD})
include_directories("${WHERE-IS-GLAD-HEADERS}")

include_directories("${CMAKE_SOURCE_DIR}/include")

# your sources
set(sources
    src/main.cpp
    include/imgui/imconfig.h
    include/imgui/imgui.cpp
    include/imgui/imgui.h
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_internal.h
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
    include/imgui/backends/imgui_impl_opengl3.cpp
    include/imgui/backends/imgui_impl_opengl3.h
    include/imgui/backends/imgui_impl_sdl.cpp
    include/imgui/backends/imgui_impl_sdl.h
)

if (WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${sources})
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        "glad"
        "../../include/sdl/lib/SDL2"
        "../../include/sdl/lib/SDL2main"
        ${OPENGL_gl_LIBRARY}
    )
else()
    add_executable(${CMAKE_PROJECT_NAME} ${sources})
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        "glad"
        ${OPENGL_gl_LIBRARY}
    )
    if (UNIX)
        target_link_libraries(
            ${CMAKE_PROJECT_NAME}
            ${SDL2_LIBRARIES}
            ${CMAKE_DL_LIBS}
        )
    endif()
endif()

# clean /bin/ directory
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# recreate /bin/ directory
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# copy font file to the binary directory
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/data/fonts/verdana.ttf
            ${CMAKE_CURRENT_BINARY_DIR}/bin/data/fonts/verdana.ttf
)

# copy SDL2.dll to the binary directory
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/include/sdl/lib/SDL2.dll
            ${CMAKE_CURRENT_BINARY_DIR}/bin/SDL2.dll
)

# move the executable from build directory to the binary directory
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.exe
            ${CMAKE_CURRENT_BINARY_DIR}/bin/${CMAKE_PROJECT_NAME}.exe
)
