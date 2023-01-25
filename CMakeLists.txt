cmake_minimum_required(VERSION 3.16.3)
project(MultClientRep)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MultClientRep
        DistanceClass.cpp
        DistanceClass.h
        Knn.cpp
        Knn.h
        Server.cpp commands.h CLI.h CLI.cpp)

add_executable(MultClientRep_ client.cpp commands.h CLI.h CLI.cpp
        DistanceClass.cpp
        DistanceClass.h
        Knn.cpp
        Knn.h)
