#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "turtle.h"

enum class Difficulty
{
  Easy,
  Medium,
  Hard
};

class Game {
 public:
  Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height, Difficulty difficulty);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;

private:
  std::unique_ptr<Turtle> pTurtle;
  std::vector<utilities::Food> foods;
  utilities::Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  utilities::Click_Message curr_click;

  const int g_width;
  const int g_height;
  const int s_width;
  const int s_height;
  int score{0};
  int hunger{0};
  int max_hunger{60};
  void PlaceFood(int x, int y);
  void PlaceRandomFood();
  void Update();
  void HandleClick();

};

#endif