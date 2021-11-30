#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include <random>
#include <queue>

#include "SDL.h"
#include "utils.h"

class Turtle {
 public:
  Turtle(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();
  void CheckForFood(int x, int y);
  void EatFood();
  void Poke();
  bool TurtleCell(int x, int y);

  // Getters
  bool GetAlive() const;
  utilities::Coordinate GetHead() const;
  int GetSize() const;

  // Setters
  void SetSpeed(int refresh_rate);
  void SetAlive(bool state);

private:
  
  // Enumerations
  enum class State
  {
    Walk,
    Turn,
    Sleep,
    Feed,
    Shake
  };
  enum class Wall
  {
    Top,
    Left,
    Right,
    Bottom,
    None
  };

  // Data classes
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
  
  // Update Functions
  void UpdateHead();
  void UpdateRotation();
  void Sleep();
  // State Transitions
  void NewDirection(Wall wall);
  void NewWalk();
  void NewShake();
  void GoSleep();
  // Helper Functions
  void DetectWall();
  TargetVector GetTargetVector(int x_start, int y_start, int x_end, int y_end);
  void ConvertToTurtleVector(TargetVector *target);
  utilities::Coordinate GetTargetCoordinate(int x, int y, int dir, int dist);

  // Constants
  const int grid_width;
  const int grid_height;

  // Constant parameters
  const int sleeepcycle{20};
  const int walking_step{1};
  const int rotation_step{5};
  const int size{8};

  // interal state variable
  State state = State::Sleep;
  bool alive{true};
  int head_x;
  int head_y;
  int rotation = 0;
  int stepping_speed = 4;

  // counters and targets
  int target_rotation_{0};
  int steps_to_go{5};
  int steps{0};
  int shakes_to_go{0};
  int counter{0};




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