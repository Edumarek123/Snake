//
// Created by Edumarek on 21/06/2021.
//

#ifndef MATRIZ_LIB_HPP
#define MATRIZ_LIB_HPP
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <assert.h>

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
    void subtrair(Matriz m);
    void multiplicar_escalar(float n);
    void multiplicar(Matriz m);
    void multiplicar_Hadamard(Matriz m);
    void transposta();
    void map(std::string fun_type);
    bool vericar_igualdade(Matriz m);
};

Matriz somar(Matriz A, Matriz B);
Matriz subtrair(Matriz A, Matriz B);
Matriz multiplicar_escalar(Matriz m, float n);
Matriz multiplicar(Matriz A, Matriz B);
Matriz multiplicar_Hadamard(Matriz A, Matriz B);
Matriz transposta(Matriz L);
Matriz map(Matriz m, std::string fun_type);
bool vericar_igualdade(Matriz A, Matriz B);

int traduzindo_escolha(std::string fun_type);
float fun_ction(float x, std::string fun_type);

#endif //MATRIZ_LIB_HPP
