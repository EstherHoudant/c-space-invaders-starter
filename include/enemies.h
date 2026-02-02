#ifndef ENEMIES_H
#include "entity.h"
#define ENEMIES_H

typedef struct
{
    Entity enemies[20];
    float vx, vy;
    int w, h;
} Horde;


#endif
//on peut appeler position si c'est un sous argument de personnage mais seulement si c'est le premie argument
// ex : affiche_position((Position*)p_hero)