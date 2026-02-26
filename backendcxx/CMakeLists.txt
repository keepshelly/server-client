cmake_minimum_required(VERSION 3.16)
project(GpsTracker)

set(CMAKE_CXX_STANDARD 17)

# --- 1. Подключаем JSON ---
add_subdirectory(third_party/json)

# --- 2. Подключаем GLFW ---
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# --- 3. Подключаем ImGui ---
set(IMGUI_DIR third_party/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# ImGui тоже использует OpenGL, поэтому подключаем его и сюда
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)


# --- 4. Подключаем ZMQ (СЕТЬ) ---
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_PERF_TOOL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ENABLE_CURVE OFF CACHE BOOL "" FORCE)
set(ENABLE_DRAFTS OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/libzmq)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/cppzmq)

# --- 5. ТВОЙ ПРОЕКТ ---
file(GLOB SOURCES "src/*.cpp")

add_executable(MyServer ${SOURCES})

target_include_directories(MyServer PRIVATE 
    include
    "${CMAKE_SOURCE_DIR}/third_party/libzmq/include"
    "${CMAKE_SOURCE_DIR}/third_party/cppzmq"
)

# --- ЛИНКОВКА (СБОРКА ВСЕГО ВМЕСТЕ) ---
target_link_libraries(MyServer PRIVATE 
    imgui 
    glfw 
    nlohmann_json::nlohmann_json
    
    # Сеть
    $<TARGET_FILE:libzmq-static>
    
    # Системные библиотеки Linux
    pthread
    dl
    
    # Графика (НОВОЕ: исправляет ошибку glViewport)
    ${OPENGL_LIBRARIES}
)