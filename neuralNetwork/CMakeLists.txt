set(SOURCE_FILE
    layer.cpp
    neuron.cpp
    network.cpp
)

add_library(NeuralNetwork ${SOURCE_FILE})
target_link_libraries(NeuralNetwork PUBLIC DataHandle)