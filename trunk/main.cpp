/*  Copyright 2008 Insanerz
    This file is part of Insanerz Shooter.

    Insanerz Shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Insanerz Shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Insanerz Shooter.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <sstream>
#include <string>
#include <math.h>
#include <SDL/SDL_mixer.h>

// Constantes de ambiente.. para poder compilar para o PSP e para PC
#define psp // descomente esta linha para compilar para o PSP

#ifdef psp

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <psppower.h>

#ifdef PSPFW3X
PSP_MODULE_INFO("InsanerzShooter", 0x0, 1, 1);
PSP_HEAP_SIZE_KB(12*1024);
#else
PSP_MODULE_INFO("InsanerzShooter", 0x1000, 1, 1);
#endif

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;
const int TITLE_X = 135;
const int TITLE_Y = 50;
const int FRAMES_PER_SECOND = 360;
#else
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TITLE_X = 310;
const int TITLE_Y = 200;
const int FRAMES_PER_SECOND = 160;
#endif

const int NUMBER_OF_BACKGROUND_STARS = 50;
const int NUMBER_OF_PARTICLES_ENEMY_EXPLOSION = 30;

using namespace std;
using std::vector;

// variaveis globais (sim, podre.. depois eu tiro isso)
int pontos = 0;
bool jogadorEstaVivo = false;
bool gamePaused = false;

#ifdef psp
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();

    return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);

    if (thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}
#endif

void writetofile(char msg[], int tecla) {
    FILE* err = fopen("errorlog.txt", "a");
    fprintf(err, "%s %i\n", msg, tecla);
    fclose(err);
}

// Método criado para calcular o valor absoluto (ou seja, sem o sinal) de um número
// @param numero número a ser avaliado
double Abs(double numero) {
    if ( numero >= 0 ) {
        return numero;
    } else {
        return -numero;
    }
}

// Classe utilizada para controle de tempo. É usada para controlar por exemplo o número de frames por segundo
class Timer {

private:

    //The clock time when the timer started
    int startTicks;
    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

public:

    Timer() {
        //Initialize the variables
        startTicks = 0;
        pausedTicks = 0;
        paused = false;
        started = false;
    }

    ~Timer() {}

    void start() {
        //Start the timer
        started = true;

        //Unpause the timer
        paused = false;

        //Get the current clock time
        startTicks = SDL_GetTicks();
    }

    void stop() {
        //Stop the timer
        started = false;

        //Unpause the timer
        paused = false;
    }

    void pause() {
        //If the timer is running and isn't already paused
        if ( ( started == true ) && ( paused == false ) ) {
            //Pause the timer
            paused = true;

            //Calculate the paused ticks
            pausedTicks = SDL_GetTicks() - startTicks;
        }
    }

    void unpause() {
        //If the timer is paused
        if ( paused == true ) {
            //Unpause the timer
            paused = false;

            //Reset the starting ticks
            startTicks = SDL_GetTicks() - pausedTicks;

            //Reset the paused ticks
            pausedTicks = 0;
        }
    }

    int get_ticks() {
        //If the timer is running
        if ( started == true ) {
            //If the timer is paused
            if ( paused == true ) {
                //Return the number of ticks when the timer was paused
                return pausedTicks;
            } else {
                //Return the current time minus the start time
                return SDL_GetTicks() - startTicks;
            }
        }

        //If the timer isn't running
        return 0;
    }

    bool is_started() {
        return started;
    }

    bool is_paused() {
        return paused;
    }

};

// Representa uma posição X,Y.
class Posicao {

public:

    // A posição é tratada como float para podermos colocar velocidades não inteiras e esse tipo de coisa.
    // Mas na hora de exibir na tela, será feito um cast pra int.
    float x, y;

    Posicao() {
        x = 0;
        y = 0;
    }

    Posicao(int novoX, int novoY) {
        x = novoX;
        y = novoY;
    }

    ~Posicao() {}

};


// Representa um Sprite; ou seja, uma imagem/surface com posição X,Y e que no futuro suportará vários frames (animação)
class IL_Sprite {

public:

    // Quando utilizamos SDL, armazenamos todas as imagens em uma Surface.
    SDL_Surface *surface;
    // Representa a posição X,Y onde o Sprite se encontra na tela.
    Posicao position;

    // Construtor padrao
    IL_Sprite() {}

    ~IL_Sprite() {}

    // Construtor
    IL_Sprite(char nomeArquivoImagem[], int larguraFrameNovo) {

        // Carregando a imagem do sprite e colocando ela em um Surface
        SDL_Surface* tempSurface = IMG_Load(nomeArquivoImagem);
        if (!tempSurface) {
            fprintf(stderr, "Não foi possível carregar a imagem %s %s:\n", nomeArquivoImagem, IMG_GetError());
            exit(-1);
        } else {
            // Este comando abaixo serve para otimizar a imagem
            surface = SDL_DisplayFormat( tempSurface );
            SDL_FreeSurface( tempSurface );
        }

        // Em todas as imagens a cor 255,0,255 ficará transparente
        Uint32 colorkey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF );
        SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey );

        // Não usado por enquanto... mais pra frente teremos animações e essa variavel será usada.
        larguraFrame = larguraFrameNovo;

        // Iniciando o Sprite na posição 0,0
        position.x = 0;
        position.y = 0;
    }

    // Construtor onde você passa direto um ponteiro de uma surface.
    // É recomenado utiliza-lo quando você vai utilizar uma mesma imagem para varios sprites, evitando assim carregar uma mesma imagem várias vezes
    IL_Sprite(SDL_Surface *surfaceNova, int larguraFrameNovo) {

        surface = surfaceNova;

        // Colocando transparencia para a cor de fundo 255,0,255
        Uint32 colorkey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF );
        SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey );

        // Não é utilizado no momento, será usado no futuro para fazer animações
        larguraFrame = larguraFrameNovo;

        // Posicionando o sprite em 0,0,
        position.x = 0;
        position.y = 0;
    }

    // Método agir será sobreescrito por outras classes.
    // Representa a ação tomada pelos sprites, como sua movimentação e seu comportamento em geral.
    virtual void agir() { }

private:

    // No futuro, cada imagem/surface poderá conter vários frames para realizar animações. No momento esta variavel não é usada.
    int larguraFrame;

};

// Representa uma nave inimiga. Nada mais é do que um Sprite com o método agir implementado e alguns novos atributos.
class Inimigo : public IL_Sprite {

public:

    float velocidade;
    float seno; // usado para fazer movimentações loucas nas naves hehe (seria bom colocar um nome melhor pra essa var)
    int tipo;

    // Construtor padrao
    Inimigo() {
        position.y = -250; // posiciona o inimigo pouco acima do topo da tela
        velocidade = rand()%100; // numero aleatorio entre 0 e 100
        velocidade = (velocidade / 100) + 1; // velocidade será aleatória entre 1 e 2
        seno=(rand()%100 / 100) - 0.5; // iniciando seno com um valor aleatorio entre -0.5 e +0.5
        tipo = 0; // indica o tipo de nave. Depdendo do tipo ela vai ter um comportamento diferente no método agir()
    }

    ~Inimigo() {}

    // Construtor
    // Para evitar ficar criando um monte de imagens, deve-se carregar a imagem antes de criar um inimigo e passar apenas uma referencia para a surface
    // O tipo indica qual será o comportamento do inimigo no método agir()
    Inimigo(IL_Sprite novoSprite, int tipoNovo) {
        tipo = tipoNovo;
        surface = novoSprite.surface;
        position.x = rand() % int((SCREEN_WIDTH - (SCREEN_WIDTH * 0.087)));
        position.y = -250;
        velocidade = rand()%100;
        velocidade = (velocidade / 100) + 1; // velocidade será aleatória entre 1 e 2
        seno=(rand()%100 / 100) - 0.5; // iniciando seno com um valor aleatorio entre -0.5 e +0.5
    }

    // Método responsável pelo comportamento do inimigo.
    // Cada tipo de inimigo tem um comportamento.
    // Talvez fosse mais interessante usar polimorfismo e dividir em várias classes os inimigos, mas tive problemas para fazer isso em C++.
    void agir() {

        if (gamePaused) {
            return;
        }

        if (tipo == 0) {// 0 = apenas anda para baixo
            position.y = position.y + velocidade;

        } else if (tipo == 1) {// 1 = zigue zague para direita
            position.y = position.y + velocidade;
            position.x = position.x + (sin(seno)) + 1;
            seno = seno + 0.1;
            if (position.x + 29 > SCREEN_WIDTH) {
                tipo = 2;
            }

        } else if (tipo == 2) {// 2 = zigue zague para esquerda
            position.y = position.y + velocidade;
            position.x = position.x + (sin(seno)) - 1;
            seno = seno + 0.1;
            if (position.x < 0) {
                tipo = 1;
            }

        } else if (tipo == 3) {// 3 = giro curto sentido horario em seguida anda reto para baixo
            if (position.y < 200) {
                position.x = position.x + (cos(seno) * 5);
                position.y = position.y + (sin(seno) * 5) + velocidade;
                seno = seno + 0.1;
            } else {
                position.y = position.y + velocidade;
            }

        } else if (tipo == 4) {// 4 = giro curto sentido anti-horario
            position.x = position.x - (cos(seno) * 5);
            position.y = position.y + (sin(seno) * 5) + velocidade;
            seno = seno + 0.1;

        } else if (tipo == 5) {// 5 = giro curto sentido horario
            position.x = position.x + (cos(seno) * 5);
            position.y = position.y + (sin(seno) * 5) + velocidade;
            seno = seno + 0.1;

        } else if (tipo == 6) {// 6 = giro longo sentido horario
            position.x = position.x + (cos(seno) * 5);
            position.y = position.y + (sin(seno) * 5) + velocidade;
            seno = seno + 0.03;

        } else if (tipo == 7) {// 7 = giro longo sentido anti-horario
            position.x = position.x - (cos(seno) * 5);
            position.y = position.y + (sin(seno) * 5) + velocidade;
            seno = seno + 0.03;
        }

        // Se sair da tela, volta para o topo em uma posição X aleatória
        if ( position.y > int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125)) ) {
            position.y = -250;
            position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
            seno = 0;
        }

    }

};

class PowerUp : public IL_Sprite {

public:

    float velocidade;
    int tipo;

    // Construtor padrao
    PowerUp() {
        position.y = -250; // posiciona o powerup acima do topo da tela
        velocidade = rand()%100; // numero aleatorio entre 0 e 100
        velocidade = (velocidade / 100) + 1; // velocidade será aleatória entre 1 e 2
        tipo = 0; // indica o tipo de powerup. Dependendo do tipo vai ter um comportamento diferente no método agir()
    }

    ~PowerUp() {}

    // Construtor
    // Para evitar ficar criando um monte de imagens, deve-se carregar a imagem antes de criar um powerup e passar apenas uma referencia para a surface
    // O tipo indica qual será o comportamento do powerup no método agir()
    PowerUp(IL_Sprite novoSprite, int tipoNovo) {
        tipo = tipoNovo;
        surface = novoSprite.surface;
        position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
        position.y = -250;
        velocidade = rand()%100;
        velocidade = (velocidade / 100) + 1; // velocidade será aleatória entre 1 e 2
    }

    void mover() {

        if (!gamePaused) {
            position.y = position.y + 0.5;
        }

    }

};

class Jogador {

public:

    IL_Sprite spriteJogador;
    int numeroDeBalas;
    int arma;
    int velocidadeArma;
    float velocidade; // velocidade da nave
    Timer *morteTimer;  // Timer para dar um "delay" entre a morte e a possibilidade de reiniciar o jogo

    Jogador() {
        velocidade = 1.5;
        arma = 0;
        velocidadeArma = 430;
        morteTimer = new Timer();
        morteTimer->start();
        numeroDeBalas = 100;
    }

    // Construtor padrão
    Jogador(IL_Sprite spriteJogadorNovo) {
        morteTimer = new Timer();
        morteTimer->start();
        spriteJogador = spriteJogadorNovo;
        arma = 0;
        velocidadeArma = 430; // é o tempo de delay entre os tiros - quanto menor o valor, mais rapida é a arma!
        numeroDeBalas = 100;
        velocidade = 1.5;
    }

    ~Jogador() {
        delete morteTimer;
        morteTimer = NULL;
    }

    void reiniciar() {
        pontos = 0;
        arma = 0;
        velocidadeArma = 430;
        numeroDeBalas = 100;
        velocidade = 1.5;
        spriteJogador.position.x = SCREEN_WIDTH / 2 - 32;
        spriteJogador.position.y = SCREEN_HEIGHT - 28;
    }

};

// Representa um tiro do jogador.
class Tiro {

public:

    int tipo;
    Posicao position;
    Uint32 corTiroVelLenta;
    Uint32 corTiroVelMedia;
    Uint32 corTiroVelRapida;
    int velocidadeTiro;
    int width, heigth;

    Tiro(int velocidadeTiroNova, int x, int y, IL_Sprite  spriteTiroNovo, int tipoNovo) {
        position.x = x;
        position.y = y;
        tipo = tipoNovo;
        width = 6;
        heigth = 12;
        velocidadeTiro = velocidadeTiroNova;
    }

    ~Tiro() {}

    void desenhar(SDL_Surface *tela) {

        SDL_Rect tiroRect;
        tiroRect.x = position.x;
        tiroRect.y = position.y;
        tiroRect.w = width;
        tiroRect.h = heigth;
        corTiroVelLenta = SDL_MapRGB( tela->format, 0x81, 0xFF, 0x1B );
        corTiroVelMedia = SDL_MapRGB( tela->format, 0xD4, 0xC4, 0x39 );
        corTiroVelRapida = SDL_MapRGB( tela->format, 0xD1, 0x1B, 0xFF );
        if (velocidadeTiro == 2) {
            SDL_FillRect(tela, &tiroRect, corTiroVelRapida);
        } else if (velocidadeTiro == 1) {
            SDL_FillRect(tela, &tiroRect, corTiroVelMedia);
        } else {
            SDL_FillRect(tela, &tiroRect, corTiroVelLenta);
        }

    }

    void atualizarPosicao() {
        if (!gamePaused) {
            // tiro para frente centralizado
            if (tipo ==0) {
                position.y = position.y - 5;
                // tiro para frente esquerda
            } else if (tipo ==1) {
                position.x = position.x - 1;
                position.y = position.y - 5;
                // tiro para direita
            } else if (tipo ==2) {
                position.x = position.x + 1;
                position.y = position.y - 5;
            }
        }
    }

};

// Particula
class Particula {

public:

    Posicao posicao;
    SDL_Rect posicaoSDL;
    float somaX;
    float somaY;
    int corParaFadeR;
    int corParaFadeG;
    int corParaFadeB;
    Uint32 cor;
    int tipo;

    Particula() {
        corParaFadeR = 0;
        corParaFadeG = 0;
        corParaFadeB = 0;
        SDL_Rect posicaoSDL = SDL_Rect();
    }

    Particula(int x, int y, int tipoNovo) {

        tipo = tipoNovo;
        posicao.x = x;
        posicao.y = y;
        somaX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
        somaY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
        if (Abs(somaX + somaY) < 0.01) {
            somaX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
            somaY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
        }

        if ( (tipo == 1 || tipo==2) && somaY < 0) {
            somaY = -somaY;
        }

        if (tipo == 0) {
            corParaFadeR = 120 + (rand()%135);
            corParaFadeG = 0;
            corParaFadeB = 0;
        } else if (tipo == 1) {
            corParaFadeR = 229 + (rand()%21);
            corParaFadeG = 148 + (rand()%40);
            corParaFadeB = 6;
        } else if (tipo == 2) {
            corParaFadeR = 255;
            corParaFadeG = 255;
            corParaFadeB = 255;
        }

    }

    ~Particula() {}

    void desenhar(SDL_Surface *tela) {

        SDL_Rect posicaoSDL = SDL_Rect();

        if (tipo == 0) {
            posicaoSDL.h = 5;
            posicaoSDL.w = 5;
        } else if (tipo == 1) {
            posicaoSDL.h = 3;
            posicaoSDL.w = 3;
        } else if (tipo == 2) {
            posicaoSDL.h = 2;
            posicaoSDL.w = 2;
        }

        posicaoSDL.x = posicao.x;
        posicaoSDL.y = posicao.y;
        cor = SDL_MapRGB( tela->format, corParaFadeR, corParaFadeG, corParaFadeB);
        SDL_FillRect(tela, &posicaoSDL, cor);

        if (!gamePaused && (tipo==0 || tipo==1)) {
            if (corParaFadeR > 0) {
                corParaFadeR--;
            }
            if (corParaFadeG > 0) {
                corParaFadeG--;
            }
        }

    }

    void atualizarPosicao() {

        if (!gamePaused) {
            if (tipo == 0) {
                posicao.x = posicao.x + (somaX / 3);
                posicao.y = posicao.y + (somaY / 3);
            } else if (tipo == 1) {
                posicao.y = posicao.y + somaY;
            } else if (tipo == 2) {
                posicao.y = posicao.y + somaY;
                if (posicao.y > SCREEN_HEIGHT) {
                    posicao.y = 0;
                    posicao.x = rand()%SCREEN_WIDTH;
                }
            }
        }

    }

};

// SistemaDeParticulas
class SistemaDeParticulas {

public:

    vector<Particula> particulas;
    int tempoDeVida, tempoDeVidaMaximo;

    SistemaDeParticulas() {
        tempoDeVida = 0;
        tempoDeVidaMaximo = 0;
    }

    SistemaDeParticulas(int x, int y, int tipoNovo, int numParticulas, int tempoDeVidaMaximoNovo) {
        tempoDeVida = 0;
        tempoDeVidaMaximo = tempoDeVidaMaximoNovo;
        if (tipoNovo == 0) {
            for (int i = 0; i < numParticulas; i++) {
                Particula p = Particula(x,y, tipoNovo);
                particulas.push_back(p);
            }
        } else if (tipoNovo == 1) {

            Particula p = Particula(x + rand()%8 + 11,y + 28, tipoNovo);
            particulas.push_back(p);

        } else if (tipoNovo == 2) {

            Particula p = Particula(x, y, tipoNovo);
            particulas.push_back(p);

        }

    }

    ~SistemaDeParticulas() {
        particulas.clear();
    }

    void desenhar(SDL_Surface *tela) {
        for (int i = 0; i < particulas.size(); i++) {
            if (!gamePaused) {
                particulas.at(i).atualizarPosicao();
            }
            particulas.at(i).desenhar(tela);
        }
        if (!gamePaused) {
            tempoDeVida++;
        }
    }

};

// GrupoDeParticulas
class GrupoDeParticulas {

public:

    vector<SistemaDeParticulas> grupo;

    GrupoDeParticulas() {}

    ~GrupoDeParticulas() {
        grupo.clear();
    }

    void adicionarSistemaParticula(int x, int y, int tipo, int quantidadeDeParticulas, int tempoDeVidaMaximo) {
        SistemaDeParticulas s = SistemaDeParticulas(x, y, tipo, quantidadeDeParticulas, tempoDeVidaMaximo);
        grupo.push_back(s);
    }

    void desenhar(SDL_Surface *tela) {
        for (int i = 0; i < grupo.size(); i++) {
            grupo.at(i).desenhar(tela);
            if (grupo.at(i).tempoDeVidaMaximo != -1 && grupo.at(i).tempoDeVida > grupo.at(i).tempoDeVidaMaximo) {
                grupo.erase(grupo.begin() + i);
            }
        }
    }

};

// Um conjunto de tiros, onde todos tem a mesma imagem/surface
class Tiros {

public:

    vector<Tiro> tiros;     // vetor onde os tiros serão armazenados
    IL_Sprite spriteTiro;   // Sprite representando os tiros

    // Construtor padrão
    Tiros() {
        spriteTiro = IL_Sprite("res/tiro.png", 2);
    }

    ~Tiros() {
        tiros.clear();
    }

    // Cria um novo tiro e o armazena no vetor de tiros
    void novoTiro(int velocidadeTiro, int x, int y, int tipo) {
        Tiro tiro = Tiro(velocidadeTiro,x,y, spriteTiro, tipo);
        tiros.push_back(tiro);
    }

    // Desenha os tiros na tela/surface
    void desenhar(SDL_Surface *tela) {
        for (int i = 0; i < tiros.size(); i++) {
            Tiro tiro = tiros.at(i);
            tiro.desenhar(tela);
        }
    }

    // Move todos os tiros para cima
    void atualizarPosicoes() {
        if (!gamePaused) {
            for (int i = 0; i < tiros.size(); i++) {
                tiros.at(i).atualizarPosicao();
                if (tiros.at(i).position.y  < 50) {
                    tiros.erase(tiros.begin() + i);
                }
            }
        }
    }

};

// Representa um conjunto de itens de powerups
class GrupoDePowerUps {

public:

    vector<PowerUp> powerups; // vetor onde são armazenas os powerups
    Jogador * jogador;

    // Construtor padrao
    GrupoDePowerUps(Jogador * jogadorNovo) {
        jogador = jogadorNovo;
    }

    ~GrupoDePowerUps() {
        powerups.clear();
    }

    // Adiciona um novo powerup ao grupo
    void adicionar(PowerUp powerUp) {
        powerups.push_back(powerUp);
    }

    // Executa o método mover() de todos os powerups e os desenha na tela/surface
    void moverEdesenhar(SDL_Surface *tela) {
        for (int i = 0; i < powerups.size(); i++) {
            powerups.at(i).mover();
            if (powerups.at(i).position.y > 0) {
                SDL_Rect rect;
                rect.x = powerups.at(i).position.x;
                rect.y = powerups.at(i).position.y;
                SDL_BlitSurface(powerups.at(i).surface, NULL, tela, &rect);
            }
        }
    }

    // Verifica se jogador pegou um powerup. Se sim, adiciona ao jogador a habilidade daquele powerup
    void verificaColisao(Mix_Chunk * powerup, Mix_Chunk * doubleshoot, Mix_Chunk * tripleshoot, Mix_Chunk * insaneshoot, Mix_Chunk * speedup) {
        // para cada powerup
        for (int i = 0; i < powerups.size(); i++) {

            // verifica colisao com jogador
            if ( Abs(jogador->spriteJogador.position.x - powerups.at(i).position.x) < + 28 && Abs(jogador->spriteJogador.position.y - powerups.at(i).position.y) < 28) {

                if (powerups.at(i).tipo == 0) { // jogador pegou um powerup para o tiro

                    jogador->numeroDeBalas = 100;
                    if (jogador->velocidadeArma > 230) {
                        Mix_PlayChannel ( -1, powerup, 0 );
                        jogador->velocidadeArma = jogador->velocidadeArma - 100;
                    } else {

                        // tocando respectivo som
                        switch (jogador->arma) {
                        case 0:
                            Mix_PlayChannel ( -1, doubleshoot, 0 );
                            break;
                        case 1:
                            Mix_PlayChannel ( -1, tripleshoot, 0 );
                            break;
                        case 2:
                            Mix_PlayChannel ( -1, insaneshoot, 0 );
                            break;
                        case 3:
                            Mix_PlayChannel ( -1, powerup, 0 );
                            break;
                        default:
                            break;
                        }

                        if (jogador->arma < 3) {
                            jogador->arma = jogador->arma + 1;
                            jogador->velocidadeArma = 430;
                        }
                    }

                } else { // jogador pegou um powerup para velocidade
                    Mix_PlayChannel ( -1, speedup, 0 );
                    if (jogador->velocidade < 4) {
                        jogador->velocidade = jogador->velocidade + 0.5;
                    }
                }

                powerups.erase(powerups.begin() + i);
            }

        }
    }

};

// Representa um conjunto de objetos da classe Inimigo
class GrupoDeInimigos {

public:

    vector<Inimigo> inimigos; // vetor onde são armazenas os inimigos

    // Construtor padrao
    GrupoDeInimigos() { }

    ~GrupoDeInimigos() {
        inimigos.clear();
    }

    // Adiciona um novo inimigo ao grupo
    void adicionar(Inimigo sprite) {
        inimigos.push_back(sprite);
    }

    // Desenha todos os inimigos na tela/surface
    // Este método normalmente não é usado
    void desenhar(SDL_Surface *tela) {
        for (int i = 0; i < inimigos.size(); i++) {
            SDL_Rect rect;
            rect.x = inimigos.at(i).position.x;
            rect.y = inimigos.at(i).position.y;
            SDL_BlitSurface(inimigos.at(i).surface, NULL, tela, &rect);
        }
    }

    // Executa o método agir() de todos os inimigos para que eles se movam, atirem, etc.
    // Este método normalmente não é usado
    void agir(SDL_Surface *tela) {
        if (!gamePaused) {
            for (int i = 0; i < inimigos.size(); i++) {
                inimigos.at(i).agir();
            }
        }
    }

    // Execura o método agir() de todos os inimigos e os desenha na tela/surface
    void agirEdesenhar(SDL_Surface *tela) {
        for (int i = 0; i < inimigos.size(); i++) {
            if (!gamePaused) {
                inimigos.at(i).agir();
            }
            if (inimigos.at(i).position.y > 0) {
                SDL_Rect rect;
                rect.x = inimigos.at(i).position.x;
                rect.y = inimigos.at(i).position.y;
                SDL_BlitSurface(inimigos.at(i).surface, NULL, tela, &rect);
            }
        }
    }

    // Cria um novo inimigo com tipo aleatório e posição X aleatória e o adiciona no vetor de inimigos
    void criarNovoInimigo(IL_Sprite spriteInimigo, int tipo) {
        spriteInimigo.position.x = rand() % SCREEN_WIDTH;
        spriteInimigo.position.y= -250;
        Inimigo inimigo = Inimigo(spriteInimigo, tipo);
        adicionar(inimigo);
    }

    // Verifica se algum dos inimigos foi atingido por um tiro
    // @param tiros objeto do tipo Tiros contendo todos os tiros que serão analisados
    void verificaColisao(Tiros *tiros, GrupoDeParticulas *grupo, Mix_Chunk * explosion) {
        // para cada inimigo
        for (int i = 0; i < inimigos.size(); i++) {

            // verifica colisao com cada tiro
            for (int j = 0; j < tiros->tiros.size(); j++) {

                // Para tornar o método mais rápido é feita uma verificação simples de colisão:
                // se os objetos estiverem a um raio menos do que 28, é por que houve colisão
                if ( Abs(tiros->tiros.at(j).position.x - inimigos.at(i).position.x) < + 28 && Abs(tiros->tiros.at(j).position.y - inimigos.at(i).position.y) < 28) {
                    grupo->adicionarSistemaParticula(tiros->tiros.at(j).position.x, tiros->tiros.at(j).position.y, 0, NUMBER_OF_PARTICLES_ENEMY_EXPLOSION, 200);
                    if (pontos < 99999) { // atualiza contador de pontos
                        pontos++;
                    }
                    inimigos.erase(inimigos.begin() + i);
                    tiros->tiros.erase(tiros->tiros.begin() + j);
                    Mix_PlayChannel ( -1, explosion, 0 );
                    break;
                }
            }

        }
    }

    // Esse metodo verifica colisao entre o jogador e um grupo de inimigos.
    // Não deveria estar aqui... num refactory ele deve mudar de lugar no futuro.
    bool verificaColisao(Jogador *jogador) {
        for (int i=0; i < inimigos.size(); i++) {
            if (Abs(jogador->spriteJogador.position.x - inimigos.at(i).position.x) < 20 && Abs(jogador->spriteJogador.position.y - inimigos.at(i).position.y) < 20) {
                return true;
            }
        }

        return false;
    }

};

// Classe responvável por exibir as coisas na tela.
class IL_Screen {

public:

    // surface que será exibida ao usuário
    SDL_Surface *surface;

    // contador de frames
    int frame;

    Uint32 corBranca;
    Uint32 corAmarela;
    Uint32 corVerde;
    Uint32 corVermelha;
    Uint32 corAzul;
    bool estaEmFullScreen;

    // Timer controlador do frames por segundo
    Timer *fps;

    IL_Screen() {
        estaEmFullScreen = false;
    }

    ~IL_Screen() {
        delete fps;
        fps = NULL;
    }

    // Construtor
    // @param telaCheia caso true, o jogo será executado em fullscreen.
    // @param resolucaoWidth da tela (padrao = 800)
    // @param resolucaoHeight da tela (padrao = 600)
    // @param bits de resolução (padrao = 32)
    IL_Screen(bool telaCheia, int resolucaoWidth = SCREEN_WIDTH, int resolucaoHeight = SCREEN_HEIGHT, int bits = 32) {

        frame = 0;
        fps = new Timer();
        fps->start();

        if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
            fprintf(stderr,"Não foi possível inicializar a SDL: %s\n", SDL_GetError());
            exit(-1);
        }

        /* Setando resolução */
        if (telaCheia == true) {
            surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_DOUBLEBUF | SDL_FULLSCREEN);
        } else {
            #ifdef psp
            surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_SWSURFACE);
            #else
            surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_DOUBLEBUF | SDL_HWSURFACE);
            #endif
        }

        if ( !surface ) {
            fprintf(stderr, "Não foi possível inicializar modo de vídeo %s\n", SDL_GetError());
            exit(-1);
        }

        corBranca = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF );
        corAmarela = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0 );
        corVerde = SDL_MapRGB( surface->format, 0, 0xFF, 0 );
        corVermelha = SDL_MapRGB( surface->format, 0xFF, 0, 0 );
        corAzul = SDL_MapRGB( surface->format, 0, 0x22, 0xFF );
        estaEmFullScreen = telaCheia;

    }

    void desenhar(IL_Sprite &sprite) {
        SDL_Rect rect;
        rect.x = sprite.position.x;
        rect.y = sprite.position.y;
        SDL_BlitSurface(sprite.surface, NULL, surface, &rect);
    }

    void atualizar() {
        fps->start();
        frame++;
        if ( fps->get_ticks() < 1000 / FRAMES_PER_SECOND ) {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps->get_ticks() );
        }
    }

    void limpar() {
        SDL_FillRect(surface, NULL, 0);
    }

    void desenharHeader() {
        SDL_Rect posicao = SDL_Rect();
        posicao.x = 0;
        posicao.y = 0;

        #ifdef psp
        posicao.h = 30;
        #else
        posicao.h = 40;
        #endif

        posicao.w = SCREEN_WIDTH;
        SDL_FillRect(surface, &posicao, corAzul);

        #ifdef psp
        posicao.y = 30;
        #else
        posicao.y = 40;
        #endif

        posicao.h = 1;
        posicao.w = SCREEN_WIDTH;
        SDL_FillRect(surface, &posicao, corBranca);
    }

    void desenharBarraFirePower(Jogador *jogador) {
        SDL_Rect posicao = SDL_Rect();
        if (jogadorEstaVivo) {
            // desenhando a barra do fire power
            posicao.x = SCREEN_WIDTH * 0.4;
            posicao.y = 30;
            posicao.h = 4;
            posicao.w = jogador->numeroDeBalas;
            if (jogador->numeroDeBalas <= 20) {
                SDL_FillRect(surface, &posicao, corVermelha);
            } else if (jogador->numeroDeBalas <= 80) {
                SDL_FillRect(surface, &posicao, corAmarela);
            } else {
                SDL_FillRect(surface, &posicao, corVerde);
            }
        }
    }

};

// Classe responsável pela entrada de comandos do usuário
class IL_Keyboard {

public:

    // Variaveis para o controle das teclas do teclado. Será true quando a tecla respectiva estiver pressionada.
    bool leftPressed, rightPressed, upPressed, downPressed, spacePressed, F11pressed, pausePressed;
    GrupoDeInimigos *grupoDeInimigos;       // Referencia para que seja possível reiniciar o jogo
    GrupoDePowerUps *grupoDePowerUps;       // Referencia para que seja possível reiniciar o jogo
    Tiros *tiros;       // Referencia para que seja possível criar um novo tiro
    Timer *shootTimer;  // Timer para colocar um "delay" entre os intervalos da criação de um novo tiro
    IL_Screen *screen;  // referencia para a screen (para poder mudar para full screen e windowed)
    SDL_Joystick *stick;
    Mix_Chunk *laser;

    // Construtor - passe como parametro o(s) objetos que serão maniputaldos pelo teclado e crie um atributo novo para cada um deles
    IL_Keyboard(IL_Screen * screenNova, Jogador * jogadorNovo, Tiros *tirosNovo, GrupoDeInimigos *grupoDeInimigosNovo, GrupoDePowerUps *grupoDePowerUpsNovo, Mix_Chunk *laserNovo) {
        jogador = jogadorNovo;
        tiros = tirosNovo;
        grupoDeInimigos = grupoDeInimigosNovo;
        grupoDePowerUps = grupoDePowerUpsNovo;
        leftPressed = false;
        rightPressed = false;
        upPressed = false;
        downPressed = false;
        spacePressed = false;
        F11pressed = false;
        pausePressed = false;
        shootTimer = new Timer();
        shootTimer->start();
        screen = screenNova;
        stick = SDL_JoystickOpen( 0 );
        laser = laserNovo;
    }

    ~IL_Keyboard() {
        SDL_JoystickClose( stick );
    }

    // Verifica se algum tecla foi pressionada ou solta.
    // Caso usuario tenha pressionado uma tecla, sua variavel respectiva torna-se true. Ficará como false caso o usuário solte a tecla.
    void verificaTeclasPressionadas() {

        if ( SDL_PollEvent(&event) != 0 ) {

            //Verificamos o tipo do evento
            switch (event.type) {
                // Caso tenha fechado a janela
            case SDL_QUIT:
                exit(0); //Fechamos a aplicação
                break;
            default:
                break;

                // Caso tenha pressionado uma tecla qualquer
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    leftPressed = true;
                    break;
                case SDLK_RIGHT:
                    rightPressed = true;
                    break;
                case SDLK_DOWN:
                    downPressed = true;
                    break;
                case SDLK_UP:
                    upPressed = true;
                    break;
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                case SDLK_F11:
                    F11pressed = true;
                    break;
                case SDLK_p:
                    if (jogadorEstaVivo) {
                        pausePressed = true;
                    }
                    break;
                case SDLK_PAUSE:
                    if (jogadorEstaVivo) {
                        pausePressed = true;
                    }
                    break;
                case SDLK_SPACE:
                    spacePressed = true;
                    break;
                default:
                    break;
                }
                break;

                // Caso tenha soltado uma tecla qualquer
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    leftPressed = false;
                    break;
                case SDLK_RIGHT:
                    rightPressed = false;
                    break;
                case SDLK_DOWN:
                    downPressed = false;
                    break;
                case SDLK_UP:
                    upPressed = false;
                    break;
                case SDLK_SPACE:
                    spacePressed = false;
                    break;
                case SDLK_F11:
                    F11pressed = false;
                    break;
                case SDLK_p:
                    pausePressed = false;
                    break;
                case SDLK_PAUSE:
                    pausePressed = false;
                    break;
                default:
                    break;
                }
                break;

                #ifdef psp
            case SDL_JOYBUTTONDOWN:
                switch (event.jbutton.button) {
                case 7:  //LEFT
                    leftPressed = true;
                    break;
                case 9: // RIGHT
                    rightPressed = true;
                    break;
                case 6: // DOWN
                    downPressed = true;
                    break;
                case 8: // UP
                    upPressed = true;
                    break;
                case 11: // start
                    if (jogadorEstaVivo) {
                        pausePressed = true;
                    }
                    break;
                case 2: // X
                    spacePressed = true;
                    break;
                default:
                    break;
                }
                break;

            case SDL_JOYBUTTONUP:
                switch (event.jbutton.button) {
                case 7:  //LEFT
                    leftPressed = false;
                    break;
                case 9: // RIGHT
                    rightPressed = false;
                    break;
                case 6: // DOWN
                    downPressed = false;
                    break;
                case 8: // UP
                    upPressed = false;
                    break;
                case 11: // start
                    pausePressed = false;
                    break;
                case 2: // X
                    spacePressed = false;
                    break;
                default:
                    break;
                }
                break;
                #endif
            }
        }
    }

    // O método verificaTeclasPressionadas() apenas seta as variaveis para true ou false.
    // O método acoesDoTeclado é o responsável por executar de fato as ações
    void acoesDoTeclado() {
        if (F11pressed == true) {
            screen->estaEmFullScreen = !screen->estaEmFullScreen;
            screen = new IL_Screen(screen->estaEmFullScreen); // Cria uma nova tela
            F11pressed = false;
        } else if (pausePressed == true) {
            gamePaused = !gamePaused;
            pausePressed = false;
        }
        if (!gamePaused && leftPressed == true && jogador->spriteJogador.position.x > 10) {
            jogador->spriteJogador.position.x = jogador->spriteJogador.position.x - jogador->velocidade;
        }
        if (!gamePaused && rightPressed == true && jogador->spriteJogador.position.x < SCREEN_WIDTH - 20) {
            jogador->spriteJogador.position.x = jogador->spriteJogador.position.x + jogador->velocidade;
        }
        if (!gamePaused && upPressed == true && jogador->spriteJogador.position.y > int((SCREEN_HEIGHT * 0.25)) ) {
            jogador->spriteJogador.position.y = jogador->spriteJogador.position.y - jogador->velocidade;
        }
        if (!gamePaused && downPressed == true && jogador->spriteJogador.position.y < SCREEN_HEIGHT - 28) {
            jogador->spriteJogador.position.y = jogador->spriteJogador.position.y + jogador->velocidade;
        }
        if (!gamePaused && spacePressed == true) {

            if (jogadorEstaVivo == false) {
                if (jogador->morteTimer->get_ticks() > 5000) {
                    // reiniciando jogo!
                    jogadorEstaVivo = true;
                    jogador->reiniciar();
                    grupoDeInimigos->inimigos.clear();
                    grupoDePowerUps->powerups.clear();
                    spacePressed  = false; // evita bug de atirar quando inicia jogo
                }
            } else {
                // verifica se ja deu tempo para atirar denovo
                if (shootTimer->get_ticks() > jogador->velocidadeArma) {
                    Mix_PlayChannel ( -1, laser, 0 );
                    shootTimer->start();
                    // Diminui o numero de balas (desde que não seja a arma inicial)
                    if (jogador->arma != 0 || jogador->velocidadeArma < 430) {
                        jogador->numeroDeBalas--;
                    }

                    int velocidadeTiro = 0;
                    if (jogador->velocidadeArma == 330) {
                        velocidadeTiro = 1;
                    } else if (jogador->velocidadeArma == 230) {
                        velocidadeTiro = 2;
                    }

                    // Se já tiver mais de 300 pontos, liberamos o tiro sextuplo! :D
                    int posX = jogador->spriteJogador.position.x;
                    int posY = jogador->spriteJogador.position.y;
                    if (jogador->arma == 3) {
                        tiros->novoTiro(velocidadeTiro, posX + 42, posY - 10, 2);
                        tiros->novoTiro(velocidadeTiro, posX + 32, posY - 10, 2);
                        tiros->novoTiro(velocidadeTiro, posX - 20, posY - 10, 1);
                        tiros->novoTiro(velocidadeTiro, posX - 10, posY - 10, 1);
                        tiros->novoTiro(velocidadeTiro, posX + 3, posY - 15, 0);
                        tiros->novoTiro(velocidadeTiro, posX + 23, posY - 15, 0);
                        // Se já tiver mais de 200 pontos, liberamos o tiro triplo! :D
                    } else if (jogador->arma == 2) {
                        tiros->novoTiro(velocidadeTiro, posX + 32, posY - 10, 2);
                        tiros->novoTiro(velocidadeTiro, posX - 10, posY - 10, 1);
                        tiros->novoTiro(velocidadeTiro, posX + 13, posY - 15, 0);
                        // Se já tiver mais de 100 pontos, liberamos o tiro duplo! :D
                    } else if (jogador->arma == 1) {
                        tiros->novoTiro(velocidadeTiro, posX + 3, posY - 15, 0);
                        tiros->novoTiro(velocidadeTiro, posX + 23, posY - 15, 0);
                        // Se tiver menos que 100 pontos, terá apenas o tiro normal
                    } else {
                        tiros->novoTiro(velocidadeTiro, posX + 13, posY - 15, 0);
                    }

                    // Verfica se acabaram as balas!
                    if (jogador->numeroDeBalas == 0) {
                        // se estiver com powerup de velocidade, aumenta intervalo entre os tiros
                        if (jogador->velocidadeArma < 430) {
                            jogador->velocidadeArma = jogador->velocidadeArma + 100;
                        } else {
                            jogador->arma--;
                        }

                        jogador->numeroDeBalas = 100;
                    }

                }
            }
        }

    }

private:

    Jogador *jogador; // Referencia para o sprite do jogador, para que possamos movimenta-lo pelo teclado
    SDL_Event event;  // Objeto usado para capturar os eventos. Coloquei como atributo para não ficar instanciando toda hora.

};

class FonteAnimada {

public:

    Posicao posicaoChar;
    bool estaSubindo;
    SDL_Surface *surface;
    float varAuxiliarCos;

    FonteAnimada () {
        estaSubindo = false;
        posicaoChar.x = 0;
        posicaoChar.y = 0;
        surface = NULL;
        varAuxiliarCos = 0;
    }

    ~FonteAnimada () {}

    FonteAnimada(char caracter[], float novoX, float novoY, bool estaSubindoNovo, TTF_Font *font) {
        estaSubindo = estaSubindoNovo;
        posicaoChar.x = novoX;
        posicaoChar.y = novoY;
        SDL_Color textColor = { 255, 255, 255 };
        surface = TTF_RenderText_Solid( font, caracter, textColor );
        varAuxiliarCos = 0;
    }

    void animar() {
        varAuxiliarCos = varAuxiliarCos - 0.3;
        if (varAuxiliarCos < 0) {
            varAuxiliarCos = 0;
        }

        if (estaSubindo) {
            posicaoChar.y = posicaoChar.y - cos(varAuxiliarCos);
            //posicaoChar.y = 10;
        } else {
            //posicaoChar.y = posicaoChar.y + cos(varAuxiliarCos);
            posicaoChar.y = posicaoChar.y + cos(varAuxiliarCos);
        }

        if (posicaoChar.y <= 0 && estaSubindo) {
            estaSubindo = false;
            varAuxiliarCos = 1;
        } else if (posicaoChar.y > 60 && !estaSubindo) {
            estaSubindo = true;
            varAuxiliarCos = 1;
        }

    }

};

int main(int argc, char *argv[]) {

    #ifdef psp
	scePowerSetClockFrequency(333, 333, 166);
    SetupCallbacks();
    /* Functions registered with atexit() are called in reverse order, so make sure that we register sceKernelExitGame() first, so that it's called last. */
    atexit(sceKernelExitGame);
    #else
    atexit(SDL_Quit);   // Avisa o sistema que antes de sair da aplicação o método SDL_QUIT deve ser executado
    #endif
    srand(time(NULL));  // Melhora o sistema de geração de números randômicos (caso contrario a sequencia dos numeros gerados é sempre a mesma)

    IL_Screen *screen = new IL_Screen(false);       // Cria uma nova tela
    SDL_ShowCursor(false);                          // Esconde o ponteiro do mouse
    SDL_WM_SetCaption("Insanerz Shooter", NULL);    // Muda o título da janela

    // iniciando sistema de som
    int frequencia = 44100;
    int canais = 2; // 1 para mono e 2 para stereo.
    int buffer = 4096;
    Uint16 formato = AUDIO_S16SYS; //16 bits stereo
    Mix_OpenAudio ( frequencia, formato, canais, buffer );
    Mix_AllocateChannels(16);
	#ifdef psp
	#else
    Mix_Music *musica = NULL;
    musica = Mix_LoadMUS ( "res/musica.ogg" );
	#endif

    // Carregando efeitos sonoros
    Mix_Chunk *powerup = NULL;
    powerup = Mix_LoadWAV ( "res/powerup.ogg" );
    Mix_Chunk *doubleshoot = NULL;
    doubleshoot = Mix_LoadWAV ( "res/doubleshot.ogg" );
    Mix_Chunk *tripleshoot = NULL;
    tripleshoot = Mix_LoadWAV ( "res/tripleshot.ogg" );
    Mix_Chunk *insaneshoot = NULL;
    insaneshoot = Mix_LoadWAV ( "res/insaneshot.ogg" );
    Mix_Chunk *laser = NULL;
    laser = Mix_LoadWAV ( "res/laser.ogg" );
    Mix_Chunk *explosion = NULL;
    explosion = Mix_LoadWAV ( "res/explosion.ogg" );
    Mix_Chunk *speedup = NULL;
    speedup = Mix_LoadWAV ( "res/speedup.ogg" );

    // Criando e posicionando o sprite do jogador (ignore o ultimo parametro, por enquanto não é usado)
    IL_Sprite spriteJogador("res/nave.png", 2);
    spriteJogador.position.x = 416;
    spriteJogador.position.y = 480;

    Jogador *jogador = new Jogador(spriteJogador);

    // Criando e sprite do inimigo (ignore o ultimo parametro, por enquanto não é usado)
    IL_Sprite spriteInimigo("res/nave_inimigo.png", 2);
    IL_Sprite spriteInimigo2("res/nave_inimigo2.png", 2);

    // Cria um grupo inicial de inimigos
    GrupoDeInimigos grupoDeInimigos = GrupoDeInimigos();

    // Cria um grupo inicial de powerups vazio
    GrupoDePowerUps grupoDePowerUps = GrupoDePowerUps(jogador);

    IL_Sprite powerUp1Sprite("res/powerup1.png", 2);

    IL_Sprite powerUp2Sprite("res/powerup2.png", 2);

    Tiros *tiros = new Tiros(); // Cria objeto responsável por controlar os tiros

    IL_Keyboard teclado = IL_Keyboard(screen, jogador, tiros, &grupoDeInimigos, &grupoDePowerUps, laser); // Cria objeto responsável por controlar o teclado

    // Cria uma fonte que será usada para exibir o nome do jogo e pontuação
    TTF_Font *font = NULL;
    TTF_Init();
    #ifdef psp
    font = TTF_OpenFont( "ariblk.ttf", 20 );
    #else
    font = TTF_OpenFont( "ariblk.ttf", 22 );
    #endif
    if (!font) {
        printf("Erro ao carregar a fonte. Está faltando o arquivo ariblk.ttf?");
        exit(-1);
    }

    // CORES
    SDL_Color corBranca = { 255, 255, 255 };
    SDL_Color corAmarela = { 255, 255, 0 };

    // Cria uma surface usada para exibir nome do jogo
    SDL_Surface *scoreSurface = NULL;
    SDL_Rect scoreRect;
    scoreSurface = TTF_RenderText_Solid( font, "SCORE", corAmarela );
    scoreRect.x = SCREEN_WIDTH * 0.05;
    scoreRect.y = 0;

    // Cria uma surface usada para exibir PAUSE
    SDL_Surface *pauseSurface = NULL;
    pauseSurface = TTF_RenderText_Solid( font, "P A U S E", corBranca );
    SDL_Rect pauseRect;
    pauseRect.x = (SCREEN_WIDTH - pauseSurface->w) / 2;
    pauseRect.y = (SCREEN_HEIGHT - pauseSurface->h) / 2;

    // Cria uma surface usada para exibir o texto "Press FIRE to start"
    TTF_Font *fontPequena = NULL;

    #ifdef psp
    fontPequena = TTF_OpenFont( "ariblk.ttf", 18 );
    #else
    fontPequena = TTF_OpenFont( "ariblk.ttf", 14 );
    #endif

    SDL_Surface *pressFireSurface = NULL;
    SDL_Rect pressFireRect;
    pressFireSurface = TTF_RenderText_Solid( fontPequena, "Press FIRE to start", corAmarela);
    pressFireRect.y = int(SCREEN_HEIGHT * 0.85);
    pressFireRect.x = (SCREEN_WIDTH - pressFireSurface->w) / 2;

    // Cria uma surface usada para exibir o texto "Press F11 toggle fullscreen"
    fontPequena = TTF_OpenFont( "ariblk.ttf", 12 );
    SDL_Surface *pressF11Surface = NULL;
    SDL_Rect pressF11Rect;
    pressF11Rect.x = 10;
    pressF11Rect.y = 580;
    pressF11Surface = TTF_RenderText_Solid( fontPequena, "Press F11 to toggle fullscreen", corBranca);

    // Logo Insanerz Shooter (parte do insanerz)
    FonteAnimada insanerz[8];
    insanerz[0] = FonteAnimada("I", 0,   0, false, font);
    insanerz[1] = FonteAnimada("n", 30,  20, false, font);
    insanerz[2] = FonteAnimada("s", 60,  40, false, font);
    insanerz[3] = FonteAnimada("a", 90,  60, false, font);
    insanerz[4] = FonteAnimada("n", 120, 40, false, font);
    insanerz[5] = FonteAnimada("e", 150, 20, false, font);
    insanerz[6] = FonteAnimada("r", 180, 0, false, font);
    insanerz[7] = FonteAnimada("z", 210, 20, false, font);
    SDL_Rect insanerzRect;

    // Logo Insanerz Shooter (parte do shooter)
    FonteAnimada shooter[7];
    shooter[0] = FonteAnimada("S", 0,   0, false, font);
    shooter[1] = FonteAnimada("h", 30,  20, false, font);
    shooter[2] = FonteAnimada("o", 60,  40, false, font);
    shooter[3] = FonteAnimada("o", 90,  60, false, font);
    shooter[4] = FonteAnimada("t", 120, 40, false, font);
    shooter[5] = FonteAnimada("e", 150, 20, false, font);
    shooter[6] = FonteAnimada("r", 180, 0, false, font);

    // Cria uma surface e um SDL_Rect (posicao) usados para exibir a pontuação do jogador
    SDL_Surface *pontosSurface = NULL;
    pontosSurface = TTF_RenderText_Solid( font, "0", corBranca );
    SDL_Rect *posicaoPontos = new SDL_Rect();
    posicaoPontos->x = SCREEN_WIDTH * 0.05 + (scoreSurface->w / 2);
    posicaoPontos->y = 22;

    // Cria uma surface e um SDL_Rect (posicao) usados para exibir a energia da arma do jogador
    SDL_Surface *bulletSurface = NULL;
    bulletSurface = TTF_RenderText_Solid( font, "BULLETS", corAmarela );
    SDL_Rect bulletRect;
    bulletRect.x = SCREEN_WIDTH * 0.4;
    bulletRect.y = 0;

    int probDeCriarInimigo; // usado para calcular probabilidade de criar um novo inimigo

    GrupoDeParticulas *grupo = new GrupoDeParticulas();

    char pontuacao[5];
	
	fontPequena = TTF_OpenFont( "ariblk.ttf", 18 );

    for (int i=0; i < NUMBER_OF_BACKGROUND_STARS; i++) {
        grupo->adicionarSistemaParticula(rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT, 2, 0, -1);
    }

	#ifdef psp
	#else
    Mix_PlayMusic(musica, 0); 
    #endif
	
    while (true) {

        screen->limpar();   // Pinta tudo de preto
        grupo->desenhar(screen->surface);               // desenha as particulas
        screen->desenharBarraFirePower(jogador);                       // Desenha a parte superior da tela
        grupoDePowerUps.moverEdesenhar(screen->surface);
        grupoDeInimigos.verificaColisao(tiros, grupo, explosion);  // Verifica se jogador matou algum inimigo
        grupoDeInimigos.agirEdesenhar(screen->surface); // Move os inimigos e desenha eles na tela

        if (!gamePaused) {
            tiros->atualizarPosicoes();                     // Move os tiros
        }

        tiros->desenhar(screen->surface);               // Desenha tiros na tela
        teclado.verificaTeclasPressionadas();           // Verifica quais teclas estão sendo pressionadas
        grupoDePowerUps.verificaColisao(powerup, doubleshoot, tripleshoot, insaneshoot, speedup);
        if (jogadorEstaVivo) {
            screen->desenhar(jogador->spriteJogador);            // Desenha jogador na tela
            // probabilidade de criar particula de "fogo" da nave
            int probDeCriarParticulaDeFogo = 6 - ((int) jogador->velocidade); // a prob de criar particula de fogo será proporcional a vel da nave
            if (!gamePaused) {
                if (rand()%probDeCriarParticulaDeFogo == 1) {
                    grupo->adicionarSistemaParticula(jogador->spriteJogador.position.x, jogador->spriteJogador.position.y, 1, 0, 40);
                }
            } else {
                SDL_BlitSurface(pauseSurface, NULL, screen->surface, &pauseRect);   // Exibe nome do jogo
            }
        } else {
            // Desenhando nome do jogo
            for (int i=0; i < 8; i++) {
                insanerzRect.x = insanerz[i].posicaoChar.x + TITLE_X;
                insanerzRect.y = insanerz[i].posicaoChar.y + TITLE_Y;
                SDL_BlitSurface(insanerz[i].surface, NULL, screen->surface, &insanerzRect);
                insanerz[i].animar();

                if (i < 7) {
                    insanerzRect.x = shooter[i].posicaoChar.x + TITLE_X + 10;
                    insanerzRect.y = shooter[i].posicaoChar.y + TITLE_Y + 100;
                    SDL_BlitSurface(shooter[i].surface, NULL, screen->surface, &insanerzRect);
                    shooter[i].animar();
                }
            }
            if (jogador->morteTimer->get_ticks() > 5000) {
                SDL_BlitSurface(pressFireSurface, NULL, screen->surface, &pressFireRect);   // Exibe "Press FIRE to start"
            }

            SDL_BlitSurface(pressF11Surface, NULL, screen->surface, &pressF11Rect);   // Exibe "Press FIRE to start"
        }

        teclado.acoesDoTeclado();	// Realiza ações do teclado (move a nave, atira, etc)

        if (jogadorEstaVivo) {
            SDL_BlitSurface(scoreSurface, NULL, screen->surface, &scoreRect);   // Exibe nome do jogo
            SDL_BlitSurface(bulletSurface, NULL, screen->surface, &bulletRect);   // Exibe nome do jogo
        }

        if (jogadorEstaVivo == true && grupoDeInimigos.verificaColisao(jogador)) {
            Mix_PlayChannel ( -1, explosion, 0 );
            jogador->morteTimer->start();
            jogadorEstaVivo = false;
            grupo->adicionarSistemaParticula(jogador->spriteJogador.position.x, jogador->spriteJogador.position.y, 0, 500, 400);
        }

        if (jogadorEstaVivo) {
            // Esta parte precisa de um refactory... Para exibir a pontuação do usuário, só consegui fazer desta maneira:
            sprintf(pontuacao,"%i",pontos);
			#ifdef psp
            pontosSurface = TTF_RenderText_Solid( fontPequena, pontuacao, corBranca );
			#else
			pontosSurface = TTF_RenderText_Solid( font, pontuacao, corBranca );
			#endif
            SDL_BlitSurface(pontosSurface, NULL, screen->surface, posicaoPontos);
            SDL_FreeSurface(pontosSurface);
        }

        // Se necessário, aguarda alguns milisegundos para manter o FPS constante
        screen->atualizar();
        // Exibe o novo cenário ao usuário
        SDL_Flip(screen->surface);

        // se tiver menos do que 50 inimigos, tem certa probabilidade de criar um novo inimigo
        if (grupoDeInimigos.inimigos.size() < 50 && !gamePaused) {
            if (pontos < 100 && grupoDeInimigos.inimigos.size() < 30) {
                probDeCriarInimigo = rand() % 100;
            } else if (pontos < 300 && grupoDeInimigos.inimigos.size() < 30) {
                probDeCriarInimigo = rand() % 90;
            } else if (pontos < 500 && grupoDeInimigos.inimigos.size() < 30) {
                probDeCriarInimigo = rand() % 70;
            } else if (pontos < 1000 && grupoDeInimigos.inimigos.size() < 30) {
                probDeCriarInimigo = rand() % 50;
            } else if (pontos < 2000 && grupoDeInimigos.inimigos.size() < 35) {
                probDeCriarInimigo = rand() % 30;
            } else if (pontos < 3000 && grupoDeInimigos.inimigos.size() < 40) {
                probDeCriarInimigo = rand() % 30;
            } else if (pontos < 4000 && grupoDeInimigos.inimigos.size() < 45) {
                probDeCriarInimigo = rand() % 25;
            } else if (grupoDeInimigos.inimigos.size() < 50) {
                probDeCriarInimigo = rand() % 10;
            }

            if (probDeCriarInimigo == 1) {
                // 0 = apenas anda para baixo
                // 1 = zigue zague para direita
                // 2 = zigue zague para esquerda
                // 3 = giro curto sentido horario em seguida anda reto para baixo
                // 4 = giro curto sentido anti-horario
                // 5 = giro curto sentido horario
                // 6 = giro longo sentido horario
                // 7 = giro longo sentido anti-horario
                if (pontos < 30) {
                    grupoDeInimigos.criarNovoInimigo(spriteInimigo, rand()%3);
                } else {
                    grupoDeInimigos.criarNovoInimigo(spriteInimigo, rand()%4);
                }
            } else if (probDeCriarInimigo == 2) {
                if (pontos > 30 && pontos < 150) {
                    if (rand()%8 == 1) {
                        grupoDeInimigos.criarNovoInimigo(spriteInimigo2, 4 + rand()%2);
                    }
                } else if (pontos > 150) {
                    grupoDeInimigos.criarNovoInimigo(spriteInimigo2, 4 + rand()%4);
                }
            }
        }

        // Probabilidade de criar um powerup novo
        int probDeCriarPowerUp = rand() % 2000;
        if (probDeCriarPowerUp == 1) {
            PowerUp p = PowerUp(powerUp1Sprite, 0);
            grupoDePowerUps.adicionar(p);
        } else if (probDeCriarPowerUp == 2 && jogador->velocidade < 4) {
            PowerUp p = PowerUp(powerUp2Sprite, 1);
            grupoDePowerUps.adicionar(p);
        }
    }

	Mix_FreeChunk(powerup);
	Mix_FreeChunk(doubleshoot);
	Mix_FreeChunk(tripleshoot);
	Mix_FreeChunk(insaneshoot);
	Mix_FreeChunk(laser);
	Mix_FreeChunk(explosion);
	Mix_FreeChunk(speedup);
	#ifdef psp
	#else
	Mix_FreeMusic(musica);
	#endif
    Mix_CloseAudio( );
    TTF_Quit();
    #ifdef psp
    sceKernelExitGame();
    #endif
    return 0;

}
