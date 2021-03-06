#ifndef PROJ_ESTADO_H
#define PROJ_ESTADO_H

#define MAXBUFFER 1024



/**
estado.h
Definição do estado i.e. tabuleiro. Representação matricial do tabuleiro.
*/


// definição de valores possiveis no tabuleiro
typedef enum { VAZIA, VALOR_X, VALOR_O, VALOR_PONTO } VALOR;

/**
Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	VALOR peca;// Peça do jogador que tem a vez para jogar
	VALOR pecaoposta; // peça do jogador oposto aquele que tem a vez
	VALOR grelha[8][8]; //tabuleiro
	char modo; // modo em que se está a jogar! 0-> manual, 1-> contra computador
} ESTADO;


void printa(ESTADO);

#endif //PROJ_ESTADO_H