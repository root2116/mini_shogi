cmake_minimum_required(VERSION 3.2)
project(ai C)
add_library(ai STATIC ./src/game_ai.c ./src/AI.c ./src/Node.c ./src/data_generator.c ./src/matrix.c ./src/tensor.c ./src/affine_tensor.c ./src/affine.c ./src/convolution.c ./src/function.c ./src/optimizer.c ./src/relu_tensor.c ./src/relu.c ./src/sigmoid.c ./src/value_net.c)

add_subdirectory(game)
add_dependencies(ai game)
target_include_directories(ai PUBLIC PUBLIC ${CMAKE_SOURCE_DIR}/ai/include ${CMAKE_SOURCE_DIR}/ai/game/include )


