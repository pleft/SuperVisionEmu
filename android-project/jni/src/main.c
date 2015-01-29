#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"

typedef struct Sprite
{
    SDL_Texture* texture;
    uint16 w;
    uint16 h;
} Sprite;

/* Adapted from SDL's testspriteminimal.c */
Sprite LoadSprite(const char* file, SDL_Renderer* renderer)
{
    Sprite result;
    result.texture = NULL;
    result.w = 0;
    result.h = 0;
    
    SDL_Surface* temp;

    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    if (temp == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return result;
    }
    result.w = temp->w;
    result.h = temp->h;

    /* Create texture from the image */
    result.texture = SDL_CreateTextureFromSurface(renderer, temp);
    if (!result.texture) {
        fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
        return result;
    }
    SDL_FreeSurface(temp);

    return result;
}

void draw(SDL_Window *window, SDL_Renderer* renderer, const Sprite sprite)
{
    int w, h;

    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect destRect = {w/2 - sprite.w/2, h/2 - sprite.h/2, sprite.w, sprite.h};
    /* Blit the sprite onto the screen */
    SDL_RenderCopy(renderer, sprite.texture, NULL, &destRect);
}

/* Main event/render loop */
int runEventLoop(SDL_Window *window, SDL_Renderer *renderer) {
    Sprite sprite = LoadSprite("sample.bmp", renderer);
    if (sprite.texture == NULL) {
        SDL_Log("Error loading sample.bmp: %s", SDL_GetError());
        return 1;
    }

    short done = 0;
    short dirty = -1;
    SDL_Event event;
    do {
        if (dirty) {
            /* Draw a gray background */
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            
            draw(window, renderer, sprite);
        
            /* Update the screen! */
            SDL_RenderPresent(renderer);

            dirty = 0;
        }

        /* Wait for events */
        SDL_WaitEvent(&event);
        do {
            switch (event.type) {
                case SDL_QUIT:
                case SDL_KEYDOWN:
                    done = -1;
                    break;
                case SDL_WINDOWEVENT:
                    if (SDL_GetWindowID(window) == event.window.windowID) {
                        if(event.window.event ==  SDL_WINDOWEVENT_RESIZED) {
                            /* SDL_WINDOWEVENT_RESIZED will trigger SDL_WINDOWEVENT_SIZE_CHANGED,
                            so ignore it in order to avoid duplicate redraws */
                            SDL_Log("SDL_WINDOWEVENT_RESIZED");
                            continue;
                        } else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                            dirty = -1;
        #ifdef ANDROID /* SDL/Android bug https://bugzilla.libsdl.org/show_bug.cgi?id=1849 */
                            SDL_Log("SDL_WINDOWEVENT_SIZE_CHANGED");
                            SDL_GL_SwapWindow(window);
        #endif
                        }
                    }
                    break;
            }

        } while (!done && SDL_PollEvent(&event));
    } while (!done);

    SDL_DestroyTexture(sprite.texture);

    return 0;
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    /* Initialize stuff */
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0) {
        SDL_Log("Error creating window or renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* run the main event loop */
    int rc = runEventLoop(window, renderer);

    /* cleanup */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return rc;
}
