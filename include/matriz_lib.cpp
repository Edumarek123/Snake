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

void Matriz::subtrair(Matriz m){
    assert(linhas==m.linhas && colunas==m.colunas);

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            matriz[i][j]-=m.matriz[i][j];
}

void Matriz::multiplicar_escalar(float n){
    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            matriz[i][j]*=n;
}

void Matriz::multiplicar(Matriz m){
    assert(colunas==m.linhas);
    float result;

    float **matriz_temp=(float**)malloc(linhas*sizeof (float*));
    for (int i = 0; i < linhas; i++)
        matriz_temp[i] = (float *) malloc(m.colunas * sizeof(float));

    for(int i=0;i<linhas;i++) {
        for (int j = 0; j < m.colunas; j++) {
            result = 0;
            for(int k=0;k<colunas; k++)
                result += (matriz[i][k] * m.matriz[k][j]);
            matriz_temp[i][j] = result;
        }
    }
    for(int i=0;i<linhas;i++)
        free(matriz[i]);
    free(matriz);

    colunas=m.colunas;
    matriz=matriz_temp;
}

void Matriz::multiplicar_Hadamard(Matriz m){
    assert(linhas==m.linhas && colunas==m.colunas);

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            matriz[i][j]*=m.matriz[i][j];
}

void Matriz::transposta() {
    int temp_lin=linhas;

    float **t=(float**)malloc(colunas*sizeof (float*));
    for (int i = 0; i < colunas; i++)
        t[i] = (float *) malloc(linhas * sizeof(float));

    for(int i=0;i<colunas;i++)
        for(int j=0;j<linhas;j++)
            t[i][j]=matriz[j][i];

    for(int i=0;i<linhas;i++)
        free(matriz[i]);
    free(matriz);

    matriz=t;
    linhas=colunas;
    colunas=temp_lin;
}

void Matriz::map(std::string fun_type){
    float value;

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++){
            value=matriz[i][j];
            matriz[i][j]=fun_ction(value, fun_type);
        }
}

bool Matriz::vericar_igualdade(Matriz m){
    assert(linhas==m.linhas && colunas==m.colunas);
    bool igualdade=true;

    for(int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            if(matriz[i][j]!=m.matriz[i][j])
                igualdade=false;
    return(igualdade);
}

//---------------------------FUNCOES---------------------------//

Matriz somar(Matriz A, Matriz B){
    assert(A.linhas==B.linhas && A.colunas==B.colunas);
    Matriz resultado(A.linhas, A.colunas);

    for(int i=0;i<A.linhas;i++)
        for(int j=0;j<A.colunas;j++)
            resultado.matriz[i][j]=A.matriz[i][j]+B.matriz[i][j];
    return (resultado);
}

Matriz subtrair(Matriz A, Matriz B){
    assert(A.linhas==B.linhas && A.colunas==B.colunas);
    Matriz resultado(A.linhas, A.colunas);

    for(int i=0;i<A.linhas;i++)
        for(int j=0;j<A.colunas;j++)
            resultado.matriz[i][j]=A.matriz[i][j]-B.matriz[i][j];
    return (resultado);
}

Matriz multiplicar_escalar(Matriz m, float n){
    Matriz resultado(m.linhas, m.colunas);

    for(int i=0;i<m.linhas;i++)
        for(int j=0;j<m.colunas;j++)
            resultado.matriz[i][j]=m.matriz[i][j]*n;
    return (resultado);
}

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

Matriz multiplicar_Hadamard(Matriz A, Matriz B){
    assert(A.linhas==B.linhas && A.colunas==B.colunas);
    Matriz resultado(A.linhas, A.colunas);

    for(int i=0;i<A.linhas;i++)
        for(int j=0;j<A.colunas;j++)
            resultado.matriz[i][j]=A.matriz[i][j]*B.matriz[i][j];
    return (resultado);
}

Matriz transposta(Matriz L) {
    Matriz T(L.colunas, L.linhas);

    for(int i=0;i<L.colunas;i++)
        for(int j=0;j<L.linhas;j++)
            T.matriz[i][j]=L.matriz[j][i];
    return (T);
}

Matriz map(Matriz m ,std::string fun_type){
    float value;
    Matriz resultado(m.linhas, m.colunas);

    for(int i=0;i<resultado.linhas;i++)
        for(int j=0;j<resultado.colunas;j++){
            value=m.matriz[i][j];
            resultado.matriz[i][j]=fun_ction(value, fun_type);
        }
    return resultado;
}

bool vericar_igualdade(Matriz A, Matriz B){
    assert(A.linhas==B.linhas && A.colunas==B.colunas);
    bool igualdade=true;

    for(int i=0;i<A.linhas;i++)
        for(int j=0;j<A.colunas;j++)
            if(A.matriz[i][j]!=B.matriz[i][j])
                igualdade=false;
    return(igualdade);
}


//----------------------FUNCOES AUXILIARES---------------------//

int traduzindo_escolha(std::string fun_type){
    int escolha;
    if(fun_type=="sigmoide")
        escolha=1;
    if(fun_type=="seno")
        escolha=2;
    if(fun_type=="cos")
        escolha=3;
    if(fun_type=="Dsigmoide")
        escolha=4;
    return(escolha);
}

float fun_ction(float x, std::string fun_type){
    float y=0;
    int escolha= traduzindo_escolha(fun_type);
    //selecionando
    switch (escolha) {
        case(1):
            y=(1/(1+exp(-x)));
            break;
        case(2):
            y=sin(x);
            break;
        case(3):
            y=cos(x);
            break;
        case(4):
            y=x*(1-x);
            break;
        default:
            assert(1==0);
    }
    return (y);
}
