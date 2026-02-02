#include <SDL2/SDL.h>
#include "game.h"
#include "enemies.h"
#include <stdio.h>

bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
}

void update(Entity *player, Horde *E, Entity *bullet, Entity *bullet_enemy, bool *bullet_active, float dt, bool *bullet_enemy_active)
{
    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    for (int i = 0; i < 20; i++)
    {
        E->enemies[i].y += E->enemies[i].vy * dt;
        if (E->enemies[i].y < 0)
            E->enemies[i].y = 0;
        if (E->enemies[i].y + E->enemies[i].h > SCREEN_HEIGHT)
            E->enemies[i].y = SCREEN_HEIGHT - E->enemies[i].h;
    }

    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
    if (*bullet_enemy_active)
    {
        bullet_enemy->y += bullet_enemy->vy * dt;
        if (bullet_enemy->y + bullet_enemy->h < 0)
            *bullet_enemy_active = false;
    }
}

void render(SDL_Renderer *renderer, Entity *player, Horde *E, Entity *bullet, Entity *bullet_enemy, bool *bullet_active, bool *running, bool *bullet_enemy_active)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (player->life == 0)
    {
        *running = false;
    }

    // fonction qui tue le joueur car touché par un alien ou fait diminuer ses points de vie car touché par une balle
    for (int i = 0; i < 20; i++)
    {
        float x = E->enemies[i].x;
        float x_ = x + (E->enemies[i].w / 2);
        float y = E->enemies[i].y;
        float y_ = y + (E->enemies[i].h) / 2;
        float x_p = player->x + player->w / 2;
        float y_p = player->y + player->h / 2;
        if (fabs(x_p - x_) < (player->w/*+E->enemies[i].w*/)/2  && fabs(y_p - y_) < (player->h/*+E->enemies[i].h*/)/ 2)
        {
            player->life = 0;
        }
        //points de vie changent car touché par balle
        /*float x = E->enemies[i].x;
        float x_ = x + (E->enemies[i].w + bullet->w) / 2;
        float y = E->enemies[i].y;
        float y_ = y + (E->enemies[i].h) / 2;
        float x_p = player->x + player->w / 2;
        float y_p = player->y + player->h / 2;
        if (fabs(x_p - x_) < player->w / 2 && fabs(y_p - y_) < player->h / 2)
        {
            player->life = 0;
        }*/
    }
    // fonction qui tues le player si les aliens arrivent en bas de l'écran
    for (int i = 0; i < 20; i++)
    {
        if (E->enemies[i].y == SCREEN_HEIGHT - ENEMIES_HEIGHT)
        { // cette condition ne marche pas tout le temps, je ne comprends pas pourquoi
            player->life = 0;
        }
    }

    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }
    if(bullet_enemy_active){
       SDL_Rect bullet_enemy_rect = {
            (int)bullet_enemy->x, (int)bullet_enemy->y,
            bullet_enemy->w, bullet_enemy->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_enemy_rect); 
    }
    // fonction qui entraîne le décès d'un ennemi
    if (*bullet_active)
    {
        for (int i = 0; i < 20; i++)
        {
            float x = E->enemies[i].x;
            float x_ = x + (E->enemies[i].w + bullet->w) / 2;
            float y = E->enemies[i].y;
            float y_ = y + (E->enemies[i].h) / 2;
            // on place aussi les coordonnées de la bullet en son centre
            float y_bullet = bullet->y + bullet->h / 2;
            float x_bullet = bullet->x + bullet->w / 2;
            if (fabs(y_bullet - y_) < E->enemies[i].h / 2 && fabs(x_bullet - x_) < E->enemies[i].w)
            {
                E->enemies[i].alive = false;
                *bullet_enemy_active = true; // les ennemis répliquent mais n'attaquent pas sinon ( ce sont des pacifistes)
            }
        }
    }
    // fonction qui permet aux ennemis de tirer
    if (*bullet_enemy_active)
    {
        int numero = 6;
        if (E->enemies[numero].alive == true)
        {
            bullet_enemy->x = E->enemies[numero].x + E->enemies[numero].w / 2 - BULLET_WIDTH / 2;
            bullet_enemy->y = E->enemies[numero].y;
            bullet_enemy->w = BULLET_WIDTH;
            bullet_enemy->h = BULLET_HEIGHT;
            bullet_enemy->vy = BULLET_SPEED;
        }
        else
        {
            numero = numero + 1; //pour la seconde fois ou l'on appelle cette fonction
        }
    }

    // fonction qui affiche les vies
    for (int i = 0; i < player->life; i++)
    {
        SDL_Rect life_rect = {
            (int)SCREEN_WIDTH - 100 + i * 30, (int)SCREEN_HEIGHT - 80,
            20, 20};
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
        SDL_RenderFillRect(renderer, &life_rect);
    }

    for (int i = 0; i < 20; i++)
    {
        if (E->enemies[i].alive == true)
        {
            SDL_Rect enemies_rect = {
                (int)E->enemies[i].x, (int)E->enemies[i].y,
                E->enemies[i].w, E->enemies[i].h};
            SDL_SetRenderDrawColor(renderer, 0, 0, 155, 255);
            SDL_RenderFillRect(renderer, &enemies_rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
