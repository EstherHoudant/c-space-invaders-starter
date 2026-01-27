#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"
#include "enemies.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0,    
        .life=1
    };
        

// on ne peut pas retourner un tableau donc pas d'étape intermédiaire qui remplit le tableau


    Horde E = {
        .w = PLAYER_WIDTH/1.2,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 10,};

        int a=0;
        for(int i=0;i<20;i++){
            E.enemies[i].y=0;
            if(a<SCREEN_WIDTH){
                E.enemies[i].x=a;
                a=a+100;}
            else{
                E.enemies[i].x=SCREEN_WIDTH-E.enemies[i].w;
            
            };
        for (int i=0;i<10;i++){
            E.enemies[i].y=100;
        }
            E.enemies[i].vx=0;
            E.enemies[i].vy=30;
            E.enemies[i].h=30;
            E.enemies[i].w=50;
            E.enemies[i].alive=true;
        }

    Entity bullet = {0}; //met 0 partout pour bullet
    bool bullet_active = false;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        update(&player,&E, &bullet, &bullet_active, dt);
        render(renderer, &player, &E,&bullet, &bullet_active,&running);
    }

    cleanup(window, renderer);
    return 0;
}
