//
// Created by Edumarek on 27/06/2021.
//

#include "rede_neural_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

RedeNeural::RedeNeural(int tam_Lx, int tam_Ly, std::vector<int> tams_hl,float alfa_learn){
	tam_layer_x=tam_Lx;
	tam_layer_y=tam_Ly;
	numero_layers=((int)tams_hl.size())+2;
	MUTATION_RATE=alfa_learn;
	score=0;

	tam_layers.push_back(tam_layer_x);
	for(int i=1;i<numero_layers-1;i++)
		tam_layers.push_back(tams_hl[i-1]);
	tam_layers.push_back(tam_layer_y);
	tam_layers.shrink_to_fit();

	for(int i=0;i<(numero_layers-1);i++)
		bias_layers.emplace_back(new Matriz(tam_layers[i+1],1));
	bias_layers.shrink_to_fit();

	for(int i=0;i<numero_layers-1;i++)
		weights_layers.emplace_back(new Matriz(tam_layers[i+1],tam_layers[i]));
	weights_layers.shrink_to_fit();

	for(int i=0;i<numero_layers-1;i++)
		saidas.emplace_back(new Matriz(tam_layers[i+1], 1));
	saidas.shrink_to_fit();

    fun_layers = (funType* )malloc((numero_layers - 1) * sizeof(funType));

    for(int i = 0; i < numero_layers - 1; i++)
        AlteraFuncaoAtivacao("tanh", i);
}

RedeNeural::RedeNeural(const RedeNeural &r){
	tam_layer_x=r.tam_layer_x;
	tam_layer_y=r.tam_layer_y;
	numero_layers=r.numero_layers;
	MUTATION_RATE=r.MUTATION_RATE;
	score=0;

	tam_layers=r.tam_layers;

	for(int i=0;i<(r.numero_layers-1);i++)
		bias_layers.emplace_back(new Matriz(*r.bias_layers[i]));
	bias_layers.shrink_to_fit();

	for(int i=0;i<r.numero_layers-1;i++)
		weights_layers.emplace_back(new Matriz(*r.weights_layers[i]));
	weights_layers.shrink_to_fit();

	for(int i=0;i<r.numero_layers-1;i++)
		saidas.emplace_back(new Matriz(*r.saidas[i]));
	saidas.shrink_to_fit();

    fun_layers = (funType* )malloc((numero_layers - 1) * sizeof(funType));
    for(int i = 0; i < numero_layers - 1; i++)
        AlteraFuncaoAtivacao("tanh", i);
}

RedeNeural::~RedeNeural(){
	for(int i=0;i<numero_layers-1;i++){
		delete weights_layers[i];
		delete bias_layers[i];
		delete saidas[i];
	}
    free(fun_layers);
}

//---------------------------METODOS---------------------------//
void RedeNeural::salvar_rede(const std::string nome){
	std::ofstream arquivo;
	arquivo.open(nome);

	if(arquivo.is_open()) {
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++)
					arquivo << weights_layers[i]->matriz[j][k] << " ";

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++)
					arquivo << bias_layers[i]->matriz[j][k] << " ";
	}else{
		assert(arquivo.is_open());
	}

	arquivo.close();
}

void RedeNeural::carregar_rede(const std::string path){
	std::fstream arquivo;
	arquivo.open(path, std::ios::in);
	std::string valor, linha;

	if(arquivo.is_open()) {
		getline(arquivo, linha);
		std::istringstream iss(linha);

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++){
					std::getline(iss, valor, ' ');
					weights_layers[i]->matriz[j][k]=std::stof(valor);
				}

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++){
					std::getline(iss, valor, ' ');
					bias_layers[i]->matriz[j][k]=std::stof(valor);
				}
	}else{
		assert(arquivo.is_open());
	}

	arquivo.close();
}

void RedeNeural::FeedFoward(Matriz input_layer){
	//z_l=W_l.X+B
	//z_l=W_l.a_(l-1)+B
	//a_l=6z_l

	for (int i = 0; i < (numero_layers-1); i++)
		delete saidas[i];

	saidas[0]=new Matriz(multiplicar(*weights_layers[0], input_layer));
	saidas[0]->somar(*bias_layers[0]);
	saidas[0]->map(fun_layers[0]);

	for (int i = 1; i < (numero_layers-1); i++) {
		saidas[i]=new Matriz(multiplicar(*weights_layers[i], *saidas[i-1]));
		saidas[i]->somar(*bias_layers[i]);
		saidas[i]->map(fun_layers[0]);
	}
	saidas.shrink_to_fit();

	covolucao_saidas(saidas[numero_layers-2]);
}

bool RedeNeural::Mutacao(float t){
	if(MUTATION_RATE<0.0001){
		MUTATION_RATE=0;
		return false;
	}

	if((rand()%1001)/1000<MUTATION_RATE){
		int maxIndex=0;
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++)
					maxIndex++;					
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++)
					maxIndex++;					
		int indexMutado=rand()%maxIndex;

		int cont=0;
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++){
					if(indexMutado==cont){
						weights_layers[i]->matriz[j][k]+=(((rand()%201)-100)/1000);
						return true;
					}
					cont++;
				}
					
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++){
					if(indexMutado==cont){
						bias_layers[i]->matriz[j][k]+=(((rand()%201)-100)/1000);
						return true;
					}
					cont++;
				}
	}
	
	return false;
}

void RedeNeural::Calcular_Score(int t){
	//Score baseado na relacao vida com comidas coletadas
	score=t*t*2;
	if(score<1)
		score=1;
}

void RedeNeural::AlteraFuncaoAtivacao(const char *F, int L){
    if(s_iguais(F, "sigmoide"))
        fun_layers[L] = sigmoide;
    else if(s_iguais(F, "ReLU"))
        fun_layers[L] = ReLU;
    else if(s_iguais(F, "tanh"))
        fun_layers[L] = Tanh;
    else
      assert(false);
}

//---------------------------FUNCOES---------------------------//

void covolucao_saidas(Matriz* m){ //editar
	float max_value=-1, posicao=0;

	for(int i=0;i<m->linhas;i++)
		if(m->matriz[i][0]>max_value){
			max_value=m->matriz[i][0];
			posicao=i;
		}

	for(int i=0;i<m->linhas;i++) {
		if (i == posicao)
			m->matriz[i][0] = 1;
		else
			m->matriz[i][0] = 0;
	}
}

int covolucao_saidasToDecimal(Matriz* m){
	covolucao_saidas(m);
	int cont=0, i=0, resultado;
	while(m->matriz[i][0]!=1){
		cont++;
		i++;
	}
	resultado=pow(2, cont);

	return	resultado;
}

RedeNeural Cruzamento(RedeNeural a, RedeNeural b){
	RedeNeural r{a};

	for (int i = 0; i < r.numero_layers - 1; i++)
	  for (int j = 0; j < r.weights_layers[i]->linhas; j++)
	    for (int k = 0; k < r.weights_layers[i]->colunas; k++){
	      if(rand()%2)
	        r.weights_layers[i]->matriz[j][k]=a.weights_layers[i]->matriz[j][k];
	      else
	        r.weights_layers[i]->matriz[j][k]=b.weights_layers[i]->matriz[j][k];
	    }
	for (int i = 0; i < r.numero_layers - 1; i++)
	  for (int j = 0; j < r.bias_layers[i]->linhas; j++)
	    for (int k = 0; k < r.bias_layers[i]->colunas; k++){
	      if(rand()%2)
	        r.bias_layers[i]->matriz[j][k]=a.bias_layers[i]->matriz[j][k];
	      else
	        r.bias_layers[i]->matriz[j][k]=b.bias_layers[i]->matriz[j][k];
	      }
	return r;
}

//----------------------FUNCOES AUXILIARES---------------------//
bool s_iguais(const char *A, const char *B){
    int i=0;

    while(A[i] != '\0' || B[i] != '\0'){
        if(A[i] != B[i])
            return false;
        i++;
    }

    return true;
}