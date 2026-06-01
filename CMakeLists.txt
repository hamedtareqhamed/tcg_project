cmake_minimum_required(VERSION 3.10)

project(CharizardProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


file(GLOB SOURCES "src/*.cpp" "*.cpp")


file(GLOB HEADERS "include/*.hpp")


add_executable(CharizardApp ${SOURCES} ${HEADERS})

target_include_directories(CharizardApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CharizardApp
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)
