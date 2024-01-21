set(SOURCE_FILE
    layer.cpp
    neuron.cpp
    network.cpp
)

add_library(neuralNetwork ${SOURCE_FILE})
target_link_libraries(neuralNetwork PUBLIC DataHandle)