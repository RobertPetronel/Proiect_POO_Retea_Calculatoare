cmake_minimum_required(VERSION 3.10)

project(Proiect)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/calculator.cpp
    src/comenzi.cpp
    src/date_fisier.cpp
    src/director.cpp
    src/file.cpp
    src/grupa.cpp
    src/proiect.cpp
    src/utilizator.cpp
)

add_executable(Proiect ${SOURCES})