//
// Created by Edumarek on 27/01/2022.
//

#include"plantas.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

Planta::Planta(int top_X, int top_Y, int bot_X, int bot_Y, int _tamanho){
  top_x=top_X;
  top_y=top_Y;
  bot_x=bot_X;
  bot_y=bot_Y;
  tamanho=_tamanho;

  devorada();

  cor={0, 255, 0};
}

void Planta::desenhar_planta(SDL_Renderer* renderizador){
	SDL_Rect r;
	r.x=x;
	r.y=y;
	r.w=tamanho;
	r.h=tamanho;

	SDL_SetRenderDrawColor(renderizador, cor[0], cor[1], cor[2], 255);
	SDL_RenderDrawRect(renderizador, &r);
	SDL_RenderFillRect(renderizador, &r);
}

void Planta::devorada(){
  do{
    x=rand()%(bot_x-top_x)+top_x;
  }while(x%tamanho!=0);
  do{
    y=rand()%(bot_y-top_y)+top_y;
  }while(y%tamanho!=0);
}

bool Planta::colisao(int pX, int pY){
  if(x==pX && y==pY)
    return true;
  return false;
}
