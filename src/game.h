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

   // Rule of 5
  Game(const Game &other);
  Game(Game &&other);
  Game &operator=(const Game &other);
  Game &operator=(Game &&other);

private:
  std::unique_ptr<Turtle> pTurtle;
  std::vector<utilities::Food> foods;
  utilities::Food food;

  utilities::Click_Message curr_click;

  const int g_width;
  const int g_height;
  const int s_width;
  const int s_height;
  const int level_hard{15};
  const int level_medium{60};
  const int level_easy{100};
  int score{0};
  int energy{0};
  int max_energy{0};
  void PlaceFood(int x, int y);
  void Update();
  void HandleClick();

};

#endif