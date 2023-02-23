#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include"include/snake.hpp"
#include"include/plantas.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

//CONFIGURACOES JANELA
int WIDTH=600;
int HEIGHT=600;
int BORDA=5;

//SLD GLOBAIS
SDL_Window* janela=nullptr; //cria uma variavel do tipo Window
SDL_Renderer* renderizador=nullptr; //cria uma variavel do tipo renderizador

//VARIAVEIS GLOBAIS
bool SIMULANDO=true, TELA_CHEIA=true, orgulho_vivo=false, player_vivo=false;
const int TAMANHO_POPULACAO=400, GERACAO_MAXIMA=2000;
int STEPS=0, FPS=1000, GERACAO_ATUAL=0;
float MELHOR_SCORE=-1, MUTATIONRATE=0.08;

//MACROS
#define gera_aleatorio POPULACAO.emplace_back(new Snake(BORDA, BORDA, WIDTH-BORDA, HEIGHT-BORDA, BORDA, MUTATIONRATE))
#define gera_planta comidas.emplace_back(new Planta(BORDA, BORDA, WIDTH-BORDA, HEIGHT-BORDA, BORDA))

//FUNCOES SDL
void inicializacao_SDL();
void encerramento_SDL();

//FUNCOES JOGO
void update();
void input();
void draw();

void cenario();
void verifica_colisao_plantas();
void verifica_colisao_cobras();

//FUNCOES ALGORITMOS GENETICOS
void iniciar_populacao(bool carregarDeArquivo);
void deletar_populacao();
void reinicia_geracao();
void realiza_backup_populacao();
float calcula_score_populacao();
void gerar_nova_populacao();
int pool_selection(int max_score);

//FUNCOES AUXILIARES
void salvar_tudo();

std::vector<Snake*> POPULACAO;
std::vector<Snake*> backup_populacao;
std::vector<Planta*> comidas;

int main(int argc, char* argv[]){
	int fps_timer, delta_fps;

	inicializacao_SDL();
	iniciar_populacao(!std::filesystem::is_empty("saves/"));

	while(SIMULANDO && GERACAO_ATUAL<=GERACAO_MAXIMA){
		fps_timer=SDL_GetTicks();

		update();
		input();
		draw();

		delta_fps=SDL_GetTicks()-fps_timer;
		if(1000/FPS>delta_fps)
			SDL_Delay(1000/FPS-delta_fps);
	}

	encerramento_SDL();
	deletar_populacao();

	return 0;
}

void inicializacao_SDL(){
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	if(TELA_CHEIA){
		WIDTH=2560;
		HEIGHT=1600;
		SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &janela, &renderizador);
	} else
		SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &janela, &renderizador);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowBordered(janela, SDL_TRUE);
	SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);
	SDL_SetWindowTitle(janela, "Evolutionary Snake");
}

void encerramento_SDL(){
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);
	SDL_Quit();
}

void update(){
	STEPS++;

	for(int i=0; i<(int)POPULACAO.size(); i++){
		POPULACAO[i]->movimentar(comidas[i]->x, comidas[i]->y);
		POPULACAO[i]->vida-=10;
	}

	verifica_colisao_plantas();
	verifica_colisao_cobras();

	if(((int)POPULACAO.size()<=0 && STEPS>0) || GERACAO_ATUAL==GERACAO_MAXIMA || STEPS>=3000)
		reinicia_geracao();
}

void input(){
	SDL_Event evento;
		while(SDL_PollEvent(&evento)){
			if(evento.type == SDL_QUIT)
				SIMULANDO=false;
			if(evento.type == SDL_KEYDOWN){
				switch (evento.key.keysym.sym) {
					case SDLK_1:
						FPS=60;
						break;
					case SDLK_2:
						FPS=1000;
						break;
					case SDLK_ESCAPE:
						SIMULANDO=false;
					default:
						break;
				}
			}
		}
}

void draw(){
	//Limpa tela
	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderClear(renderizador);

	cenario();

	for(int i=0; i<(int)POPULACAO.size(); i++){
		POPULACAO[i]->desenhar_Snake(renderizador);
		comidas[i]->desenhar_planta(renderizador);
	}

	//Renderiza
	SDL_RenderPresent(renderizador);
}

void cenario(){
	SDL_Rect bordaE, bordaI;

	bordaE.x=0;
	bordaE.y=0;
	bordaE.w=WIDTH;
	bordaE.h=HEIGHT;

	bordaI.x=BORDA;
	bordaI.y=BORDA;
	bordaI.w=WIDTH-(2*BORDA);
	bordaI.h=HEIGHT-(2*BORDA);

	SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderizador, &bordaE);
	SDL_RenderFillRect(renderizador, &bordaE);

	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderizador, &bordaI);
	SDL_RenderFillRect(renderizador, &bordaI);
}

void iniciar_populacao(bool carregarDeArquivo){
	for(int i=0; i<TAMANHO_POPULACAO; i++){
		gera_aleatorio;
		
		if(carregarDeArquivo){
			std::string t="saves/rede_neural_" + std::to_string(i) + ".txt";
			POPULACAO[i]->cerebro->carregar_rede(t);
		}

		gera_planta;
		comidas[i]->cor=POPULACAO[i]->cor;
	}
	POPULACAO.shrink_to_fit();
	comidas.shrink_to_fit();
}

void deletar_populacao(){
	for(int i=0;i<(int)POPULACAO.size();i++)
		delete POPULACAO[i];

	for(int i=0;i<(int)backup_populacao.size();i++)
		delete backup_populacao[i];

	for(int i=0;i<(int)comidas.size();i++)
		delete comidas[i];
}

void reinicia_geracao(){
	STEPS=0;
	GERACAO_ATUAL++;
	gerar_nova_populacao();
	if(!std::filesystem::is_empty("saves/"))
		system("rm saves/*.txt");
	salvar_tudo();
}

void realiza_backup_populacao(){
	while((int)POPULACAO.size()>0){
		backup_populacao.emplace_back(new Snake(*POPULACAO[0]));
		delete POPULACAO[0];
		delete comidas[0];
		POPULACAO.erase(POPULACAO.begin());
		comidas.erase(comidas.begin());
	}
}

float calcula_score_populacao(){
	float max_score=-1;
	for(int i=0;i<(int)backup_populacao.size();i++){
		backup_populacao[i]->cerebro->Calcular_Score(backup_populacao[i]->comprimento-5);
		if(backup_populacao[i]->cerebro->score>max_score){
			max_score=backup_populacao[i]->cerebro->score;
			if(max_score>MELHOR_SCORE)
				MELHOR_SCORE=max_score;
		}
	}
	std::cout<<"Best Score = "<<MELHOR_SCORE<<" | Best Score da geracao "<<GERACAO_ATUAL<<" = "<<max_score<<std::endl;

	return max_score;
}

void gerar_nova_populacao(){
	realiza_backup_populacao();

	float max_score=calcula_score_populacao();

	//Rotina reproducao
	for(int i=0;i<TAMANHO_POPULACAO;i++)
		POPULACAO.emplace_back(new Snake(*backup_populacao[pool_selection(max_score)], *backup_populacao[pool_selection(max_score)]));

	//Limpeza backup_populacao
	for(int i=0;i<(int)backup_populacao.size();i++)
		delete backup_populacao[i];
	backup_populacao.clear();

	//Rotina criacao plantas
	for(int i=0;i<TAMANHO_POPULACAO;i++){
		gera_planta;
		comidas[i]->cor=POPULACAO[i]->cor;
	}
}

int pool_selection(int max_score){
	int indice;
	while(true){
		indice=rand()%((int)backup_populacao.size());
		if(rand()%max_score<backup_populacao[indice]->cerebro->score)
			return indice;
	}
}

void verifica_colisao_plantas(){
	for(int i=0;i<(int)POPULACAO.size();i++)
		if(comidas[i]->colisao(POPULACAO[i]->x, POPULACAO[i]->y)){
			POPULACAO[i]->comprimento++;
			POPULACAO[i]->vida=3000;
			comidas[i]->devorada();
		}
}

void verifica_colisao_cobras(){
	for(int i=0;i<(int)POPULACAO.size();i++){
		if(POPULACAO[i]->colisao_corpo())
			POPULACAO[i]->vida=-1;

		if(POPULACAO[i]->colisao_borda()){
			backup_populacao.emplace_back(new Snake(*POPULACAO[i]));
			delete POPULACAO[i];
			delete comidas[i];
			POPULACAO.erase(POPULACAO.begin()+i);
			comidas.erase(comidas.begin()+i);
			i=0;
		}
	}
}

void salvar_tudo(){
	for(int i=0;i<(int)POPULACAO.size();i++){
		std::string t="saves/rede_neural_" + std::to_string(i) + ".txt";
		POPULACAO[i]->cerebro->salvar_rede(t);
	}
}
