#pragma once

#ifndef PROJ_JOGAR_H
#define PROJ_JOGAR_H

#include "estado.h"

void interpretador(ESTADO e);
ESTADO capture_pieces(ESTADO e, int i, int j);
ESTADO make_next_move(ESTADO e, int row, int column);
ESTADO change_current_player(ESTADO e);
void display_wrong_move();
ESTADO display_score(ESTADO e);


#endif //PROJ_JOGAR_H
