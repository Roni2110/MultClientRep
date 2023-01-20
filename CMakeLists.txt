cmake_minimum_required(VERSION 3.17)
project(MultClientRep)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MultClientRep
        DistanceClass.cpp
        DistanceClass.h
        Knn.cpp
        Knn.h
        server.cpp commands.h CLI.h)

add_executable(MultClientRep_ client.cpp)
