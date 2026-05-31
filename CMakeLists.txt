cmake_minimum_required(VERSION 3.10)

project(CharizardProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES
    main.cpp
    charizardx.cpp
    charizardy.cpp
)

set(HEADERS
    main.hpp
    charizardx.hpp
    charizardy.hpp
)

add_executable(CharizardApp ${SOURCES} ${HEADERS})

target_link_libraries(CharizardApp 
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)
