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
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
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

void Renderer::Render(Snake const snake, helper::Food const &food, Turtle const turtle) {
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
  SDL_Rect flagRect;
  // Scale the turtle dimensions to fit the screen
  flagRect.w = block.w * turtle.size;
  flagRect.h = flagRect.w;
  // turtle position: In the middle of the screen
  flagRect.x = (static_cast<int>(turtle.head_x) * block.w);// -(2 * block.w);
  flagRect.y = (static_cast<int>(turtle.head_y) * block.h);
 // -(2 * block.h);
  // Render Turtle
  SDL_RenderCopyEx(sdl_renderer, flag, NULL, &flagRect, turtle.rotation, NULL, SDL_FLIP_NONE);


  // Render snake's body
  
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  
 
  /* Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }*/
  //SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
