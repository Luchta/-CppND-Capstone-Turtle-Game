#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height)
    : turtle(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      curr_click(),
      food(),
      g_width(grid_width),
      g_height(grid_height),
      s_width(screen_width),
      s_height(screen_height) {
  PlaceRandomFood();

}

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
    renderer.Render(food, turtle);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceRandomFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by an item before placing
    if (!turtle.TurtleCell(x, y)) {
      PlaceFood(x, y);
      return;
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
      if (turtle.TurtleCell(grid_x, grid_y))
      {
        turtle.Poke();
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
    food.point.x = x;
    food.point.y = y;
    food.active = true;
}

void Game::Update() {
  if (!turtle.alive) return;

  turtle.Update();

  if (food.active){
    turtle.CheckForFood(food.point.x, food.point.y);
  }

  // Check if there's food over here
  if (turtle.TurtleCell(food.point.x, food.point.y) && food.active) {
    score++;
    food.active = false;
  }
}

int Game::GetScore() const { return score; }