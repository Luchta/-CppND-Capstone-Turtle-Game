#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include <random>
#include <queue>


#include "SDL.h"

class Turtle {
 public:
  enum class Direction2 
  { 
    k0 =  0, 
    k45 = 45,
    k90 = 90,
    k135 = 135,
    k180 = 180,
    k225 = 255,
    k270 = 270,
    k315 = 315
  };
  enum class State
  {
    Walk,
    Turn,
    Sleep,
    Feed
  };
  enum class Wall
  {
    Top,
    Left,
    Right,
    Bottom,
    None
  };

  class Instruction {
    public:
      Instruction(State action, int steps) : action_(action), steps_(steps) {}
      
      State action_;
      int steps_;
      bool completed = false;
  };

  class TargetVector {
    public:
      TargetVector(int dir_, int dist_) : dir(dir_), dist(dist_) {}
      int dir;
      int dist;
  };

  class Coordinate {
    public:
      Coordinate(int x_, int y_) : x(x_), y(y_) {}
      int x;
      int y;
  };

  Turtle(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  bool TurtleCell(int x, int y);
  void CheckForFood(int x, int y);


  //Direction2 direction = Direction2::k0;

  int speed{1};
  bool alive{true};
  int head_x;
  int head_y;
  int rotation = 0;
  int size = 8;
  std::vector<SDL_Point> body;

  int update_steps = 0;
  int speed_steps = 2;

private:
  void RandomMove();
  void ChaseFood();
  void GoSleep();
  void Sleep();
  void UpdateHead();
  void UpdateRotation();
  void DetectWall();
  void NewDirection(Wall wall);
  void NewWalk();
  TargetVector GetTargetVector(int x_start, int y_start, int x_end, int y_end);
  void ConvertToTurtleVector(TargetVector *target);
  Coordinate GetTargetCoordinate(int x, int y, int dir, int dist);

  Direction2 direction = Direction2::k0;
  State state = State::Sleep;

  int target_rotation_ = 0;

  int grid_width;
  int grid_height;
  int steps_to_go = 5;
  int steps = 0;

  bool turn_left = false;
  int sleeepcycle = 20;
  int counter = 0;

   
  std::deque<Instruction> motion_path;
  /*
  std::random_device dev2;
  std::mt19937 engine2;
  std::uniform_int_distribution<int> random_dir;

  engine2(dev2()),
  random_dir(0,7) 
*/
};

#endif //TURTLE_H