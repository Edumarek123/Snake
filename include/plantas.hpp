//
// Created by Edumarek on 27/01/2022.
//

#ifndef Planta_HPP
#define Planta_HPP
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>

struct Planta{
    //posicao
    int x;	//centro da Planta
    int y;	//centro da Planta
    int tamanho;
    //bordas da tela
    int top_x, top_y, bot_x, bot_y;
    //caracteristicas
    std::vector<int> cor;

    Planta()=delete;
    Planta(int top_X, int top_Y, int bot_X, int bot_Y, int _tamanho);

    void desenhar_planta(SDL_Renderer* renderizador);
    void devorada();
    bool colisao(int pX, int pY);

};

#endif //Planta_HPP
