#!/bin/bash

clear

g++ -o model main.cpp include/matriz_lib.cpp include/rede_neural_lib.cpp include/snake.cpp include/plantas.cpp -m64 -g -I"include" -L"lib" -Wall -lSDL2main -lSDL2 -lSDL2_image

./model
# valgrind --leak-check=full --track-origins=yes -v ./model

rm model

# shutdown
