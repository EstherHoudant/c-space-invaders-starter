#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "enemies.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define ENEMIES_WIDTH PLAYER_WIDTH / 1.2
#define ENEMIES_HEIGHT PLAYER_HEIGHT

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Horde *E, Entity *bullet, Entity *bullet_enemy,bool *bullet_active, float dt,bool *bullet_enemy_active);
void render(SDL_Renderer *renderer, Entity *player, Horde *E, Entity *bullet, Entity *bullet_enemy,bool *bullet_active, bool *running,bool *bullet_enemy_active);

void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void impact();

#endif
