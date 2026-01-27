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

void update(Entity *player, Horde *E, Entity *bullet, bool *bullet_active, float dt)
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
}

void render(SDL_Renderer *renderer, Entity *player, Horde *E, Entity *bullet, bool *bullet_active,bool *running)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (player->life==0){
        *running=false; 
    }

    //fonction qui fait varier les points de vie(life) du joueur
    for (int i = 0; i < 20; i++){
        float x= E->enemies[i].x;
        float x_=x+(E->enemies[i].w+bullet->w)/2; 
        float y = E->enemies[i].y;
        float y_=y+(E->enemies[i].h)/2;
        float x_p=player->x+player->w/2;
        float y_p=player->y+player->h/2;
    if(fabs(x_p-x_)<player->w/2 && fabs(y_p-y_)<player->h/2){
    player->life=player->life-1;
    }}

    
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
// fonction qui entraîne le décès d'un ennemi
    if (*bullet_active)
    {
        for (int i = 0; i < 20; i++)
        {
            float x= E->enemies[i].x;
            float x_=x+(E->enemies[i].w+bullet->w)/2; 
            float y = E->enemies[i].y;
            float y_=y+(E->enemies[i].h)/2;
            if (fabs(bullet->y-y_)<E->enemies[i].h/2 && fabs(bullet->x-x_)<E->enemies[i].w+bullet->w/2)
            {
                E->enemies[i].alive = false;
            }
        }
    }
    
   
    //fonction qui affiche les vies 
    for (int i=0;i<player->life;i++){
        SDL_Rect life_rect ={
            (int) SCREEN_WIDTH -100+i*30, (int) SCREEN_HEIGHT -80,
            20,20};
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
