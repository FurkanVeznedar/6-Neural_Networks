cmake_minimum_required(VERSION 3.12)
project(Test)

set(SUB_DIRECTORIES
    DataHandle
    knn
    kMeans
    neuralNetwork
)

foreach(subdir ${SUB_DIRECTORIES})
    add_subdirectory(${subdir})
endforeach()


add_executable(Test main.cpp)

target_compile_definitions(Test PRIVATE EUCLID)

target_link_libraries(Test PUBLIC knn)
target_link_libraries(Test PUBLIC kMeans)
target_link_libraries(Test PUBLIC neuralNetwork)