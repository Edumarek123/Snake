//
// Created by Edumarek on 21/01/2022.
//

#ifndef SNAKE_LIB_HPP
#define SNAKE_LIB_HPP
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "matriz_lib.hpp"
#include "rede_neural_lib.hpp"
#include <cmath>

struct Snake{
    RedeNeural *cerebro;
    //posicao
    int x;
    int y;
    int tamanho;
    int top_x, top_y, bot_x, bot_y;
    //caracteristicas
    std::vector<int> cor;
    std::vector<int> corpo_x;
    std::vector<int> corpo_y;
    int comprimento;
    int vida;
    std::vector<int> direcao;

    Snake()=delete;
    Snake(int top_X, int top_Y, int bot_X, int bot_Y, int _tamanho, float mutationRate);
    Snake(const Snake &c);
    Snake(const Snake &a, const Snake &b);
    ~Snake();

    void desenhar_Snake(SDL_Renderer* renderizador);
    void debug_Snake();
    bool colisao_corpo();
    bool colisao_borda();
    void movimentar(int a, int b);
};

#endif //SNAKE_LIB_HPP
