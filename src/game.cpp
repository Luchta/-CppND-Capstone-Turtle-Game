#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height, Difficulty difficulty)
    : curr_click(),
      food(),
      g_width(grid_width),
      g_height(grid_height),
      s_width(screen_width),
      s_height(screen_height) {
  
  pTurtle = std::unique_ptr<Turtle>(new Turtle(grid_width, grid_height));
  food.active = false;
  curr_click.active = false;

  switch (difficulty)
  {
    case Difficulty::Easy:
      max_energy = level_easy;
      energy = level_easy;
      break;
    case Difficulty::Medium:
      max_energy = level_medium;
      energy = level_medium;
      break;
    case Difficulty::Hard:
      max_energy = level_hard;
      energy = level_hard;
      break;
  }

}

/* Rule of 5 ******************************************************************/


Game::Game(const Game &other)
    : g_width(other.g_width),
      g_height(other.g_height),
      s_width(other.s_width),
      s_height(other.s_height) {
    std::cout << "Game Copy Constructor" << std::endl;
    pTurtle = std::unique_ptr<Turtle>(new Turtle(g_width, g_height));
}

Game::Game(Game &&other)
    : g_width(other.g_width),
      g_height(other.g_height),
      s_width(other.s_width),
      s_height(other.s_height) {
    std::cout << "Game Move Constructor" << std::endl;

    // move turtle from heap memory
    this->pTurtle = std::move(other.pTurtle);
    other.pTurtle = nullptr;
}

Game& Game::operator=(const Game& other) 
{
    std::cout << "Game Copy Assignment Operator" << std::endl;
    if(this == &other) {
        return *this;
    }

    return *this = Game(other);
}

 Game& Game::operator=(Game &&other)
{
    std::cout << "Game Move Assignment Operator" << std::endl;
    if(this == &other) {
        return *this;
    }

    // move turtle from heap memory
    this->pTurtle = std::move(other.pTurtle);
    other.pTurtle = nullptr;
    return *this;
}

/* Interactions ***************************************************************/

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, curr_click);
    HandleClick();
    Update();
    renderer.Render(food, pTurtle.get());

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every half second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(energy, score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
      // Update Energy if alive 
      if(pTurtle->GetAlive()){
        energy--;
      }
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::HandleClick() {
  if (curr_click.active)
  {
    bool valid_x = ((curr_click.coord.x >= 0) && (curr_click.coord.x <= s_width));
    bool valid_y = ((curr_click.coord.y >= 0) && (curr_click.coord.y <= s_height));
    if (valid_x && valid_y)
    {
      int grid_x = curr_click.coord.x / (s_width / g_width);
      int grid_y = curr_click.coord.y / (s_height / g_height);
      if (pTurtle->TurtleCell(grid_x, grid_y))
      {
        pTurtle->Poke();
        energy = energy - 10;
      }
      else
      {
        PlaceFood(grid_x, grid_y);
      }
    }
    else
    {
      std::cout << "Invalid click!" << curr_click.coord.x<< "," << curr_click.coord.y<< "\n ";
    }
  curr_click.active = false;
  }
}

void Game::PlaceFood(int x, int y) {
  if(!food.active){
    food.point.x = x;
    food.point.y = y;
    food.active = true;
  }
}

void Game::Update() {
  if (!pTurtle->GetAlive()) return;

  pTurtle->Update();

  if (food.active){
    pTurtle->CheckForFood(food.point.x, food.point.y);
  }

  if(energy <= 0){
    pTurtle->SetAlive(false);
  }

  // Check if there's food over here
  if (pTurtle->TurtleCell(food.point.x, food.point.y) && food.active) {
    score++;
    energy = max_energy;
    pTurtle->SetSpeed(3);
    food.active = false;
  }
}

int Game::GetScore() const { return score; }