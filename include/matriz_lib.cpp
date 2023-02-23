//
// Created by Edumarek on 26/06/2021.
//
#include "matriz_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

Matriz::Matriz(){
  linhas=0;
  colunas=0;
  matriz=NULL;
}

Matriz::Matriz(int lin, int col){
    linhas=lin;
    colunas=col;

    matriz=(float**)malloc(linhas*sizeof (float*));

    for (int i = 0; i < linhas; i++)
        matriz[i] = (float *) malloc(colunas * sizeof(float));
    carrega_valores(0, true);
}

Matriz::Matriz(const Matriz &m){
  linhas=m.linhas;
  colunas=m.colunas;
  matriz=(float**)malloc(linhas*sizeof (float*));

  for (int i = 0; i < linhas; i++)
      matriz[i] = (float *) malloc(colunas * sizeof(float));

  for(int i=0;i<linhas;i++)
    for(int j=0;j<colunas;j++)
      matriz[i][j]=m.matriz[i][j];
}

Matriz::~Matriz(){
   for(int i=0;i<linhas;i++)
       free(matriz[i]);
   free(matriz);
}



//---------------------------METODOS---------------------------//

void Matriz::carrega_valores(float n, bool aleatorio){
    if(aleatorio)
        for(int i=0;i<linhas;i++)
            for(int j=0;j<colunas;j++)
                do{matriz[i][j]=((float)(rand()%201-100)/100);}while(matriz[i][j]==0);
    else
        for(int i=0;i<linhas;i++)
            for(int j=0;j<colunas;j++)
                matriz[i][j]=n;
}

void Matriz::imprimir(){
    for(int i=0;i<linhas;i++){
        for(int j=0;j<colunas;j++)
            std::cout<< matriz[i][j] << " ";
        std::cout<<std::endl;
    }
}

void Matriz::somar(Matriz m){
    assert(linhas==m.linhas && colunas==m.colunas);

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            matriz[i][j]+=m.matriz[i][j];
}

void Matriz::multiplicar_escalar(float n){
    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            matriz[i][j]*=n;
}

void Matriz::map(const funType f){
    float value;

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++){
            value=matriz[i][j];
            matriz[i][j]=f(value);
        }
}

//---------------------------FUNCOES---------------------------//

Matriz multiplicar(Matriz A, Matriz B){
    assert(A.colunas==B.linhas);
    Matriz resultado(A.linhas, B.colunas);
    float produto;

    for(int i=0;i<A.linhas;i++) {
        for (int j = 0; j < B.colunas; j++) {
            produto = 0;
            for(int k=0;k<A.colunas; k++)
                produto += (A.matriz[i][k] * B.matriz[k][j]);
            resultado.matriz[i][j] = produto;
        }
    }
    return(resultado);
}

//----------------------FUNCOES DE ATIVACAO--------------------//

float sigmoide(float x){
    return 1 / (1 + exp(-x));
}

float Dsigmoide(float x){
    return x * (1 - x);
}

float ReLU(float x){
    if(x > 0)
        return x;
    else
        return 0;
}

float DReLU(float x){
    if(x > 0)
        return 1;
    else
        return 0;
}

float Tanh(float x){
    return tanh(x);
}

float DTanh(float x){
    return 1 - (tanh(x) * tanh(x));
}

float seno(float x){
    return sin(x);
}

//----------------------FUNCOES AUXILIARES---------------------//
