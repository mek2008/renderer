#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "objects.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>
#include <array>
#include <limits>








struct Framebuffer {
int width, height;
std::vector<Pixel> pixels;

Framebuffer(int w, int h)
    : width(w), height(h), pixels(w*h)  {}

Pixel& at(int x, int y) {return pixels[y * width + x]; }

};

struct SDLState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int height, width;
    SDLState(int width, int height) : width(width), height(height) {}
};

void cleanup(SDLState &state){
 if (state.texture) SDL_DestroyTexture(state.texture);
 if (state.renderer) SDL_DestroyRenderer(state.renderer);
 if (state.window) SDL_DestroyWindow(state.window);
    SDL_Quit();
}





int main(int argc, char *argv[])
{
SDLState state(1600, 900); 
Perspective persptv({0, 0, 0}, 1);

//SDL_Init(SDL_INIT_VIDEO);
if (!SDL_Init(SDL_INIT_VIDEO)) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initiatlizing SDL3", nullptr); return -1; }
state.window = SDL_CreateWindow("SDL3 demo", state.width, state.height, 0);

if (!state.window){ SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window); cleanup(state); return -1; }

// create the renderer
state.renderer = SDL_CreateRenderer(state.window, nullptr);
if(!state.renderer) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window); cleanup(state); return -1;}



Framebuffer framebuffer(state.width, state.height);

state.texture = SDL_CreateTexture(
    state.renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    state.width, state.height 
    );


bool running = true; while (running) {

    SDL_Event event{ 0 };
    while (SDL_PollEvent(&event)) {
    switch (event.type){
        case SDL_EVENT_QUIT:
        {
            running = false;
            break;
        }

    }
    }
 
//simple clean up
SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
SDL_RenderClear(state.renderer);
std::fill(framebuffer.pixels.begin(), framebuffer.pixels.end(), Pixel{0,0,0,255});



for (int y = 0; y < framebuffer.height; y++) {
for (int x = 0; x < framebuffer.width; x++) {

float aspect = (float)framebuffer.width / framebuffer.height;

float u = (2.0f * x / framebuffer.width - 1.0f) * aspect;
float v = (1.0f - 2.0f * y / framebuffer.height);

Ray ray;
ray.origin = persptv.position; 
ray.direction = persptv.localToGlobal(u, v, persptv.focalLength);

float length = sqrt(u*u + v*v + persptv.focalLength*persptv.focalLength);
Vec3 localDir = {u/length, v/length, persptv.focalLength/length};

ray.direction = persptv.localToGlobal(u, v, persptv.focalLength);
ray.direction = normalize3v(ray.direction);

// ray.direction = {(u/length) + persptv.pitch, (v/length) + persptv.yaw, (-persptv.focalLenght/length)};
//worldDir = rotate(localDir, camera.q);



//--------------------------------------------------


//--------------------------------------------------
}
}






//unkown type return so void pointers are used, a werid sdl3 feture
void* pixels;
int pitch;

//SDL_LockTexture(state.texture, nullptr, &pixels, &pitch);
if (SDL_LockTexture(state.texture, nullptr, &pixels, &pitch) < 0) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error with texture lock", state.window);
        cleanup(state);
        return -1;
}

for (int y = 0; y < framebuffer.height; y++) {

    unsigned char* row = (unsigned char*)pixels + y * pitch;

    for (int x = 0; x < framebuffer.width; x++) {

        Pixel &p = framebuffer.at(x,y);

        row[x*4 + 0] = p.r;
        row[x*4 + 1] = p.g;
        row[x*4 + 2] = p.b;
        row[x*4 + 3] = p.a;
    }
}
//unlock, render and present, its literlay wirtten in the name
SDL_UnlockTexture(state.texture);
SDL_RenderTexture(state.renderer, state.texture, nullptr, nullptr);
SDL_RenderPresent(state.renderer);
}
    



    cleanup(state); return 0;
}