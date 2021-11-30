#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "turtle.h"
#include "utils.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(utilities::Food const &food, Turtle const* turtle);
  void UpdateWindowTitle(int energy, int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Texture *flag;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif