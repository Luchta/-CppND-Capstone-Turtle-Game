#include "renderer.h"
#include <iostream>
#include <string>
#include <SDL_image.h>


Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Turtle Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Load Texture
  #define TN_FLAG_IMAGE_PATH "assets/turtle.png"

  flag = IMG_LoadTexture(sdl_renderer, TN_FLAG_IMAGE_PATH);
  if(!flag)
  {
      printf("Unable to load image '%s'!\n"
              "SDL_image Error: %s", TN_FLAG_IMAGE_PATH, IMG_GetError());
      //return false;
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(utilities::Food const &food, Turtle const* turtle) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  if(food.active){
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.point.x * block.w;
    block.y = food.point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }


  // Turtle dimensions/position
  SDL_Rect turtleRect;
  utilities::Coordinate turtleHead = turtle->GetHead();
  // Scale the rect to size of turtle
  turtleRect.w = block.w * turtle->GetSize();
  turtleRect.h = turtleRect.w;
  // turtle position according to grid(block) size 
  turtleRect.x = (turtleHead.x) * block.w;
  turtleRect.y = (turtleHead.y) * block.h;
  // red if dead
  if(!turtle->GetAlive()){
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(sdl_renderer, &turtleRect);
  }
  // Render Turtle in rect
  SDL_RenderCopyEx(sdl_renderer, flag, NULL, &turtleRect, turtleHead.rot, NULL, SDL_FLIP_NONE);
  
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int hunger, int score, int fps) {
  std::string title{"Hunger: " + std::to_string(hunger) + " Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}