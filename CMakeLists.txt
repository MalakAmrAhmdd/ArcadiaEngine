cmake_minimum_required(VERSION 3.10)

project(ArcadiaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    ArcadiaEngine.cpp
    main_test_student.cpp
)

set(HEADERS
    ArcadiaEngine.h
)

add_executable(ArcadiaEngine ${SOURCES} ${HEADERS})
