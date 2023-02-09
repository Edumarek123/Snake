//
// Created by Edumarek on 22/01/2022.
//
#include "snake.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

Snake::Snake(int top_X, int top_Y, int bot_X, int bot_Y, int _tamanho){
	cerebro=new RedeNeural(10, 4, {64}, 0.08);

	bot_x=bot_X;
	bot_y=bot_Y;
	top_x=top_X;
	top_y=top_Y;

	x=(bot_x+top_x)/2;
	y=(bot_y+top_y)/2;

	tamanho=_tamanho;
	comprimento=tamanho;
	vida=bot_x-top_y;
	vida=3000;

	cor.push_back(rand()%256);
	cor.push_back(rand()%256);
	cor.push_back(rand()%256);
	cor.shrink_to_fit();

	direcao.push_back(0);
	direcao.push_back(0);

	corpo_x.push_back(x);
	corpo_y.push_back(y);
	for(int i=1;i<tamanho;i++){
		corpo_x.push_back(corpo_x[i-1]+tamanho);
		corpo_y.push_back(corpo_y[i-1]+tamanho);
	}
}

Snake::Snake(const Snake &c){
	cerebro=new RedeNeural(*c.cerebro);

	bot_x=c.bot_x;
	bot_y=c.bot_y;
	top_x=c.top_x;
	top_y=c.top_y;

	tamanho=c.tamanho;
	comprimento=c.comprimento;
	vida=3000;

	cor=c.cor;

	direcao=c.direcao;

	x=(bot_x+top_x)/2;
	y=(bot_y+top_y)/2;

	corpo_x.push_back(x);
	corpo_y.push_back(y);
	for(int i=1;i<tamanho;i++){
		corpo_x.push_back(corpo_x[i-1]+tamanho);
		corpo_y.push_back(corpo_y[i-1]+tamanho);
	}
}

Snake::Snake(const Snake &a, const Snake &b){
	cerebro=new RedeNeural(Cruzamento(*a.cerebro, *b.cerebro));

	bot_x=a.bot_x;
	bot_y=a.bot_y;
	top_x=a.top_x;
	top_y=a.top_y;

	x=(bot_x+top_x)/2;
	y=(bot_y+top_y)/2;

	tamanho=a.tamanho;
	comprimento=tamanho;
	vida=3000;

	for(int i=0;i<3;i++){
		if(rand()%2)
			cor.push_back(a.cor[i]);
		else
			cor.push_back(b.cor[i]);
	}
	cor.shrink_to_fit();

	if(cerebro->Mutacao(1)){
		if(rand()%101<20)
			cor[rand()%((int)cor.size())]=rand()%256;
	}

	direcao=a.direcao;

	corpo_x.push_back(x);
	corpo_y.push_back(y);
	for(int i=1;i<tamanho;i++){
		corpo_x.push_back(corpo_x[i-1]+tamanho);
		corpo_y.push_back(corpo_y[i-1]+tamanho);
	}
}

Snake::~Snake(){
	delete cerebro;
}
//--------------------------METODOS---------------------------//
void Snake::desenhar_Snake(SDL_Renderer* renderizador){
	while((int)corpo_x.size()>comprimento){
		corpo_y.pop_back();
		corpo_x.pop_back();
	}

	for(int i=0; i<comprimento;i++){
		SDL_Rect r;
		r.x=corpo_x[i];
		r.y=corpo_y[i];
		r.w=tamanho;
		r.h=tamanho;

		SDL_SetRenderDrawColor(renderizador, cor[0], cor[1], cor[2], 255);
		SDL_RenderDrawRect(renderizador, &r);
		SDL_RenderFillRect(renderizador, &r);
	}
}

void Snake::debug_Snake(){
	std::cout<<"x = "<<x<<" | y = "<<y<<" | tamanho = "<<tamanho<<std::endl;
	std::cout<<"R = "<<cor[0]<<" | G = "<<cor[1]<<" | B = "<<cor[2]<<std::endl;
}

bool Snake::colisao_corpo(){
	for(int i=1;i<comprimento;i++)
		if(x==corpo_x[i] && y==corpo_y[i])
			return true;
	return false;
}

bool Snake::colisao_borda(){
	if(x>=bot_x || x<top_x || y>=bot_y || y<top_y || vida<=0)
		return true;
	return false;
}

void Snake::movimentar(int a, int b){
	Matriz* entradas=new Matriz(10, 1);
	entradas->carrega_valores(-1, false);

	//angulo da comida
	float cX=(a-x);
	float cY=(b-y);
	float cM=sqrt((cX*cX)+(cY*cY));

	entradas->matriz[0][0]=cX/cM;
	entradas->matriz[1][0]=cY/cM;
	//visao parede
	if(x+tamanho==bot_x)
		entradas->matriz[2][0]=1;
	if(y+tamanho==bot_y)
		entradas->matriz[3][0]=1;
	if(x-tamanho==top_x)
		entradas->matriz[4][0]=1;
	if(y-tamanho==top_y)
		entradas->matriz[5][0]=1;
	if(x+tamanho==bot_x && y+tamanho==bot_y)
		entradas->matriz[6][0]=1;
	if(x-tamanho==top_x && y-tamanho==top_y)
		entradas->matriz[7][0]=1;
	if(x+tamanho==bot_x && y-tamanho==top_y)
		entradas->matriz[8][0]=1;
	if(x-tamanho==top_x && y+tamanho==bot_y)
		entradas->matriz[9][0]=1;
	//visao corpo
	for(int i=1;i<comprimento;i++){
		if(x+tamanho==corpo_x[i])
			entradas->matriz[2][0]=1;
		if(y+tamanho==corpo_y[i])
			entradas->matriz[3][0]=1;
		if(x-tamanho==corpo_x[i])
			entradas->matriz[4][0]=1;
		if(y-tamanho==corpo_y[i])
			entradas->matriz[5][0]=1;
		if(x+tamanho==corpo_x[i] && y+tamanho==corpo_y[i])
			entradas->matriz[6][0]=1;
		if(x-tamanho==corpo_x[i] && y-tamanho==corpo_y[i])
			entradas->matriz[7][0]=1;
		if(x+tamanho==corpo_x[i] && y-tamanho==corpo_y[i])
			entradas->matriz[8][0]=1;
		if(x-tamanho==corpo_x[i] && y+tamanho==corpo_y[i])
			entradas->matriz[9][0]=1;
	}

	cerebro->FeedFoward(*entradas);

	delete entradas;

	int escolha=covolucao_saidasToDecimal(cerebro->saidas[cerebro->numero_layers-2]);
	switch (escolha){
		case 1:
			if(direcao[0]!=-1){
				direcao[0]=1;
				direcao[1]=0;
			}
			break;
		case 2:
			if(direcao[1]!=-1){
				direcao[0]=0;
				direcao[1]=1;
			}
			break;
		case 4:
			if(direcao[0]!=1){
				direcao[0]=-1;
				direcao[1]=0;
			}
			break;
		case 8:
			if(direcao[1]!=1){
				direcao[0]=0;
				direcao[1]=-1;
			}
			break;
	}

	x+=(direcao[0]*tamanho);
	y+=(direcao[1]*tamanho);

	corpo_x.insert(corpo_x.begin(), x);
	corpo_y.insert(corpo_y.begin(), y);
}
