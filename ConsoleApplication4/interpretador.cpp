#define _CRT_SECURE_NO_DEPRECATE
#include "estado.h"
#include "interpretador.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int playable_direction[8][8][8];
int has_valid_move = 0;//necessario para quando o jogador nao tem jogadas validas
int scores[2];
int wrong_move = 0;
int distance(int i1, int j1, int i2, int j2)
{
	int di = abs(i1 - i2), dj = abs(j1 - j2);
	if (di > 0) return di;
	return dj;
}

int jogadavalida(int i, int j) // VER SE A JOGADA ESTA DENTRO DO TABULEIRO
{
	if (i < 0 || i >= 8 || j < 0 || j >= 8) return 0;
	return 1;
}

ESTADO novojogo(ESTADO e, char *peca) {
	// estado inicial do tabuleiro. Inicio do jogo!
	e.grelha[3][4] = VALOR_O;
	e.grelha[4][3] = VALOR_O;
	e.grelha[3][3] = VALOR_X;
	e.grelha[4][4] = VALOR_X;
	switch (toupper(peca[0])) {
	case 'X':
		e.peca = VALOR_X;
		e.pecaoposta = VALOR_O;
		break;
	case 'O':
		e.peca = VALOR_O;
		e.pecaoposta = VALOR_X;
		break;
	default:
		printf("a peca tem de ser X ou O \n");
	}
	return e;
}

int ejogavel(ESTADO e, int i, int j) { //

	memset(playable_direction[i][j], 0, 8);
	if (!jogadavalida(i, j)) return 0;
	if (e.grelha[i][j] != VAZIA) return 0;
	int possiveljogada = 0;


	// Test UL diagonal
	int i_it = i - 1, j_it = j - 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
	{
		i_it -= 1;
		j_it -= 1;
	}
	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][0] = 1;
		possiveljogada = 1;
	}

	// Test UP path
	i_it = i - 1, j_it = j;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
		i_it -= 1;

	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][1] = 1;
		possiveljogada = 1;
	}

	// Test UR diagonal
	i_it = i - 1, j_it = j + 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
	{
		i_it -= 1;
		j_it += 1;
	}
	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][2] = 1;
		possiveljogada = 1;
	}

	// Test LEFT path
	i_it = i, j_it = j - 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
		j_it -= 1;

	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][3] = 1;
		possiveljogada = 1;
	}

	// Test RIGHT path
	i_it = i, j_it = j + 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
		j_it += 1;

	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][4] = 1;
		possiveljogada = 1;
	}

	// Test DL diagonal
	i_it = i + 1, j_it = j - 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
	{
		i_it += 1;
		j_it -= 1;
	}
	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][5] = 1;
		possiveljogada = 1;
	}

	// Test DOWN path
	i_it = i + 1, j_it = j;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
		i_it += 1;

	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][6] = 1;
		possiveljogada = 1;
	}

	// Test DR diagonal
	i_it = i + 1, j_it = j + 1;
	while (jogadavalida(i_it, j_it) && e.grelha[i_it][j_it] == e.pecaoposta)
	{
		i_it += 1;
		j_it += 1;
	}
	if (jogadavalida(i_it, j_it) && distance(i, j, i_it, j_it) > 1 && e.grelha[i_it][j_it] == e.peca)
	{
		playable_direction[i][j][7] = 1;
		possiveljogada = 1;
	}
	return possiveljogada;


}

ESTADO marcajogadas(ESTADO e) { // MARCAR . NAS JOGADAS VALIDAS

	has_valid_move = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (e.grelha[i][j] == VALOR_PONTO)
			{//apaga os pontos do tabuleiro porque eles nao sao peças validas e vao ser substituidos por novos pontos
				e.grelha[i][j] = VAZIA;
			}
			if (ejogavel(e, i, j) == 1)
			{
				e.grelha[i][j] = VALOR_PONTO;
				has_valid_move = 1;
			}
		}
	}
	return e;
}

ESTADO change_current_player(ESTADO e) // trocar de jogador
{
	if(e.peca == VALOR_O)
	{
		e.peca = VALOR_X;
		e.pecaoposta = VALOR_O;
	}else
	{
		e.peca = VALOR_O;
		e.pecaoposta = VALOR_X;
	}

	return e;
}

void display_wrong_move()
{
	if (wrong_move)
	{
		printf("You entered an invalid move!\n");
		wrong_move = 0;
	}
}

ESTADO capture_pieces(ESTADO e, int i, int j) {

	int i_it, j_it;

	// Capture UL diagonal
	if (playable_direction[i][j][0])
	{
		i_it = i - 1, j_it = j - 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it -= 1;
			j_it -= 1;
		}
	}

	// Capture UP path
	if (playable_direction[i][j][1])
	{
		i_it = i - 1, j_it = j;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it -= 1;
		}
	}

	// Capture UR diagonal
	if (playable_direction[i][j][2])
	{
		i_it = i - 1, j_it = j + 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it -= 1;
			j_it += 1;
		}
	}

	// Capture LEFT path
	if (playable_direction[i][j][3])
	{
		i_it = i, j_it = j - 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			j_it -= 1;
		}
	}

	// Capture RIGHT path
	if (playable_direction[i][j][4])
	{
		i_it = i, j_it = j + 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			j_it += 1;
		}
	}

	// Capture DL diagonal
	if (playable_direction[i][j][5])
	{
		i_it = i + 1, j_it = j - 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it += 1;
			j_it -= 1;
		}
	}

	// Capture DOWN path
	if (playable_direction[i][j][6])
	{
		i_it = i + 1, j_it = j;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it += 1;
		}
	}

	// Capture DR diagonal
	if (playable_direction[i][j][7])
	{
		i_it = i + 1, j_it = j + 1;
		while (e.grelha[i_it][j_it] == e.pecaoposta)
		{
			e.grelha[i_it][j_it] = e.peca;
			scores[e.peca]++;
			scores[e.pecaoposta]--;
			i_it += 1;
			j_it += 1;
		}
	}
	return e;
}

ESTADO make_next_move(ESTADO e, int i, int j) // JOGAR 
{
	if (jogadavalida(i, j) && e.grelha[i][j] == VALOR_PONTO)
	{
		e.grelha[i][j] = e.peca;
		scores[e.peca]++;
		e = capture_pieces(e, i, j);
		e = change_current_player(e);
	}
	else wrong_move = 1;
	return e;
}

ESTADO interpretar(ESTADO e, char *linha) {
	char cmd[MAXBUFFER];
	char peca[MAXBUFFER];
	int n;
	int l, c;

	sscanf(linha, "%s", cmd);
	switch (toupper(cmd[0])) { // toupper é usado para validar maiusculas e minusculas
	case 'N': // N peca novo jogo em que o primeiro a jogar é o jogador com peça
		n = sscanf(linha, "%s %s", cmd, peca);
		if (n == 2) e = novojogo(e, peca); // Se o n tiver 2 parametros -> executar novo jogo
		else printf("erro: e preciso dois parametros\n");
		break;
	case 'L': // L ficheiro para ler um jogo de ficheiro
		break;
	case 'E': // E ficheiro escrever em ficheiro estado do jogo.
		break;
	case 'J': // J l c jogar peça atual na posicao (L,C)
		n = sscanf(linha, "%s %d %d", cmd, &l, &c);
		if (n == 3) e = make_next_move(e, l-1, c-1);
		else printf("erro: e preciso tres parametros\n");
		e = marcajogadas(e);
		break;
	case 'S': // S imprimir um ponto . nas posicoes com jogada valida.
		e = marcajogadas(e);
		break;
	case 'H'://para sugestão de jogada. Deve ser colocado um ‘?’ no sitio sugerido.
		break;
	case 'U'://para desfazer a última jogada(Undo).Isto tem de permitir desfazer até ao estado inicial do jogo!
		break;
	case 'A'://A <peça> <nível> novo jogo contra ‘bot’ em que o computador joga com a peça <peça> contra um bot de nível <nível> (3 níveis possíveis). Neste modo quem joga primeiro é sempre o jogador com a peça preta ‘X’.
		break;
	case 'Q':// Sair do jogo
		exit(0);
	default:
		printf("comando invalido(rtfm)!\n");
	}
	return e;
}

void printprompt(ESTADO e) {
	switch (e.peca) {
	case VALOR_X://vez do jogador X
		printf("reversi X> ");
		break;
	case VALOR_O://Vez do joagador O
		printf("reversi O> ");
		break;
	default: //Sem jogador para fazer proxima jogada
		printf("reversi ?> ");
	}


}

void interpretador(ESTADO e) {
	char linha[MAXBUFFER];
	printprompt(e);
	while (fgets(linha, MAXBUFFER, stdin)) {
		e = interpretar(e, linha);
		printa(e);
		display_wrong_move();
		printprompt(e);
	}
}
