cmake_minimum_required(VERSION 3.20)
project(HW1)

set(CMAKE_CXX_STANDARD 11)

add_executable(HW1 main.cpp
        include/Action.h
        include/Customer.h
        src/Action.cpp
        src/Customer.cpp
        src/Studio.cpp
        src/Trainer.cpp src/Workout.cpp)
