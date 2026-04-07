#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "objects.hpp"
#include "perspNmath.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <atomic>



struct Framebuffer {
int width, height;
std::vector<Pixel> pixels;

Framebuffer(int w, int h)
    : width(w), height(h), pixels(w*h)  {}

Pixel& at(int x, int y) {return pixels[y * width + x]; }

void smoothing(bool simplesmoothing){

std::vector<Pixel> temp(height * width);

if(simplesmoothing){
for (int i = 0; i < height; i++){
for (int j = 0; j < width; j++){

if((i == 0 || i == (height-1))  || (j == 0 || j == (width -1))){

    temp[i * width + j].r = pixels[i * width + j].r;
    temp[i * width + j].g = pixels[i * width + j].g;
    temp[i * width + j].b = pixels[i * width + j].b;

    } else {
            temp[i * width + j].r = static_cast<unsigned char>(pixels[(i-1) * width + j].r*0.20)  +  static_cast<unsigned char>(pixels[i * width + j-1].r*0.20)  +  
                                    static_cast<unsigned char>(pixels[i * width + j+1].r*0.20)    +  static_cast<unsigned char>(pixels[(i+1) * width + j].r*0.20) 
                                    +  static_cast<unsigned char>(pixels[i * width + j].r*0.20);

            temp[i * width + j].g = static_cast<unsigned char>(pixels[(i-1) * width + j].g*0.20)  +  static_cast<unsigned char>(pixels[i * width + j-1].g*0.20)  +  
                                    static_cast<unsigned char>(pixels[i * width + j+1].g*0.20)    +  static_cast<unsigned char>(pixels[(i+1) * width + j].g*0.20) 
                                    +  static_cast<unsigned char>(pixels[i * width + j].g*0.20);

            temp[i * width + j].b = static_cast<unsigned char>(pixels[(i-1) * width + j].b*0.20)  +  static_cast<unsigned char>(pixels[i * width + j-1].b*0.20)  +  
                                    static_cast<unsigned char>(pixels[i * width + j+1].b*0.20)    +  static_cast<unsigned char>(pixels[(i+1) * width + j].b*0.20) 
                                    +  static_cast<unsigned char>(pixels[i * width + j].b*0.20);
            temp[i * width + j].a = pixels[i * width + j].a;

            if(temp[i * width + j].r >255) temp[i * width + j].r = 255;
            if(temp[i * width + j].g >255) temp[i * width + j].g = 255;
            if(temp[i * width + j].b >255) temp[i * width + j].b = 255;
            }
    }
}
pixels = temp;
}

    return;
};
};


struct SDLState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int width, height;
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

//-------------------------------------
SDLState state(1600, 900); 
Perspective persptv({1, 0, 0}, 1);
std::vector<std::unique_ptr<Object>> scene;
float ambient = 0.1f;
bool antiAliasing = true;
unsigned char antiAliasingm = 50;
bool fsmoothing = false;
unsigned char threadCount = 15;
persptv.rotatetioni(0.0f);
persptv.rotatetionj(0.0f);
persptv.rotatetionk(0.0f);
//-------------------------------------
//-----------------------------------------

scene.push_back(std::make_unique<Sphere>(Vec3{0,0,3}, 0.5f, Pixel{255,0,0,255}));
scene.push_back(std::make_unique<Sphere>(Vec3{-1.2f, 0.9f, 2.55f}, 0.2f, Pixel{0,255,0,255}));
//scene.push_back(std::make_unique<Tetrahedron>(Vec3{-1,-1,3}, Vec3{1,-1,3}, Vec3{0,1,3}, Vec3{0,0,1}, Pixel{0,255,255,255}));

std::vector<std::unique_ptr<LightSource>> lights;
lights.push_back(std::make_unique<LightSource>(Vec3{3, 5, 0}, 200, Pixel{255, 255, 255, 255}));
lights.push_back(std::make_unique<LightSource>(Vec3{-6, 5, 2}, 150, Pixel{255, 255, 200, 255}));
// add more lights if needed
//-----------------------------------------


//SDL_Init(SDL_INIT_VIDEO);
if (!SDL_Init(SDL_INIT_VIDEO)) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initiatlizing SDL3", nullptr); return -1; }
state.window = SDL_CreateWindow("renderer", state.width, state.height, 0);

if (!state.window){ SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window); cleanup(state); return -1; }

// create the renderer
state.renderer = SDL_CreateRenderer(state.window, nullptr);
if(!state.renderer) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window); cleanup(state); return -1;}


Framebuffer framebuffer(state.width, state.height);

state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,         state.width, state.height );



//--------------------------------




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
 
//------------------------------------
SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
SDL_RenderClear(state.renderer);
std::fill(framebuffer.pixels.begin(), framebuffer.pixels.end(), Pixel{0,0,0,255});



std::vector<std::thread> treads;
std::vector<int> jobs;
for (int i = 0; i < framebuffer.height; i++) jobs.push_back(i);
std::atomic<int> jobIndex = 0;


float aspect = (float)framebuffer.width / framebuffer.height;
float antiAliasingmsq = 1 / static_cast<float>(antiAliasingm * antiAliasingm);


auto work = [&](){
while(1 == 1){

    int y = jobIndex.fetch_add(1);
    if (y >= framebuffer.height) return;

for (int x = 0; x < framebuffer.width; x++) {

// ray initilization
float u = (2.0f * x / framebuffer.width - 1.0f) * aspect;
float v = (1.0f - 2.0f * y / framebuffer.height);

Ray ray;
ray.origin = persptv.position; 
ray.direction = persptv.localToGlobal(u, v, persptv.focalLength);
ray.direction = normalize3v(ray.direction);




//--------------------------------------------------

ClosestP closest;
for (const auto& obj : scene)
    closest.closestH(obj->hitRR(ray));

if (closest.hit) { // instead of checking t < max
    framebuffer.at(x, y) = shadowImplementation(scene, closest.colCord, lights, ambient);
}
//-------------------------------------------------
if (antiAliasing){
float tempr = 0, tempg = 0, tempb = 0;

for (unsigned i = 0; i < antiAliasingm; i++){
for (unsigned j = 0; j < antiAliasingm; j++){
// ray initilization
float u = (2.0f * (x + (i / static_cast<float>(antiAliasingm))) / (framebuffer.width) - 1.0f) * aspect;
float v = (1.0f - 2.0f * (y + (j/ static_cast<float>(antiAliasingm))) / (framebuffer.height));


Ray ray;
ray.origin = persptv.position; 
ray.direction = persptv.localToGlobal(u, v, persptv.focalLength);
ray.direction = normalize3v(ray.direction);
ClosestP closestAA;
for (const auto& obj : scene)
    closestAA.closestH(obj->hitRR(ray));

    if (closestAA.hit) { // instead of checking t < max
        Pixel result = shadowImplementation(scene, closestAA.colCord, lights, ambient);
        tempr += result.r * antiAliasingmsq;
        tempg += result.g * antiAliasingmsq;
        tempb += result.b * antiAliasingmsq;
    }
}
}
framebuffer.at(x, y) = {static_cast<unsigned char>(tempr), static_cast<unsigned char>(tempg), static_cast<unsigned char>(tempb), framebuffer.at(x, y).a };
//only safe if Each thread gets a unique y and No two threads write the same pixel
}

};
}
};

for (int t = 0; t < static_cast<int>(threadCount); t++) treads.emplace_back(work);
for (auto& t : treads) t.join(); treads.clear();
jobIndex = 0;

/*
SDL_UpdateTexture(
    state.texture,
    nullptr,
    framebuffer.pixels.data(),
    framebuffer.width * sizeof(Pixel)
); 
*/

void* pixels;
int pitch;
framebuffer.smoothing(fsmoothing);

//SDL_LockTexture(state.texture, nullptr, &pixels, &pitch);
if (!SDL_LockTexture(state.texture, nullptr, &pixels, &pitch)) {
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