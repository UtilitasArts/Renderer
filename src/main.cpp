#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>

#include "Time.h"

bool is_running     = false;
SDL_Window*   window   = NULL;
SDL_Renderer* renderer = NULL;

bool InitializeWindow(void) {    
    if (!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr,"Unable to initialize SDL\n");
        return false;
    }

    //create a window
    window = SDL_CreateWindow("Renderer", 800, 600, 0);

    if(!window){ 
    fprintf(stderr,"Error Creating Window"); 
    return false;
    }

    //Create SDL renderer
    renderer = SDL_CreateRenderer(window, NULL);    
    if(!renderer){
        fprintf(stderr,"Error Creating Renderer\n");
        return false;
    }
    return true; 
}

void Setup(void)
{

}

void ProcessInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
        case SDL_EVENT_QUIT:
            is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE)
            is_running = false;
            break;
    }    
}

void Update(void)
{
    
}

void Render(void) {
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

int main(void) {  

    is_running = InitializeWindow();
    Setup();

    while(is_running)
    {
        Time::Manager.TakeTimeStep();        
        while( Time::Manager.ShouldUpdate() )
        { 
            Time::Manager.IncTimeStep();  

            ProcessInput();
            Update();
            Render();
        }              
    }



    // while(GlobalTimer->getElapsedNanoSeconds() < 5 * Nanoseconds) {    
    //     std::cout << "Elapsed time: " << GlobalTimer->getElapsedNanoSeconds() / (float)Nanoseconds << " seconds" << std::endl;

    //    std::cout << (uint64_t)1000000000/(uint64_t)60 << std::endl;

    // }
        
    

    is_running =InitializeWindow();    


    return 0;    



}