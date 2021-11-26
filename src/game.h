#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "turtle.h"

class Game {
 public:
  Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  Snake snake;
  Turtle turtle;
  helper::Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  helper::Click_Message curr_click;

  const int g_width;
  const int g_height;
  const int s_width;
  const int s_height;
  int score{0};
  void PlaceFood(int x, int y);
  void PlaceRandomFood();
  void Update();
  void HandleClick();

};

#endif