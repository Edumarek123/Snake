//
// Created by Edumarek on 21/06/2021.
//

#ifndef MATRIZ_LIB_HPP
#define MATRIZ_LIB_HPP
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <assert.h>

typedef float (*funType)(float);

struct Matriz{
    int linhas;
    int colunas;
    float **matriz;

    Matriz();
    Matriz(int lin, int col);
    Matriz(const Matriz &m);
    ~Matriz();

    void carrega_valores(float n, bool aleatorio);
    void imprimir();

    void somar(Matriz m);
    void multiplicar_escalar(float n);
    void map(const funType f);
};

Matriz multiplicar(Matriz A, Matriz B);

float sigmoide(float x);
float Dsigmoide(float x);
float ReLU(float x);
float DReLU(float x);
float Tanh(float x);
float DTanh(float x);
float seno(float x);

#endif //MATRIZ_LIB_HPP
