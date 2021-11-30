#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include <random>
#include <queue>

#include "SDL.h"
#include "utils.h"

class Turtle {
 public:
   Turtle(int grid_width, int grid_height);

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
   enum class Action
   {
     Walk,
     Turn,
     Sleep
   };
   enum class Wall
   {
     Top,
     Left,
     Right,
     Bottom,
     None
   };
  enum class State
   {
     Moving,
     Feeding,
     Shaking
   };

   // Data classes
   class Instruction
   {
   public:
     Instruction(Action action, int steps) : action_(action), steps_(steps) {}

     Action action_;
     int steps_;
     bool completed = false;
  };
  class TargetVector {
    public:
      TargetVector(int dir_, int dist_) : dir(dir_), dist(dist_) {}
      int dir;
      int dist;
  };
  
  // State Functions
  void Walk();
  void Turn();
  void Sleep();
  void Feed();
  void Shake();

  // Update Functions
  void UpdateHead();
  void UpdateRotation();
  // State Transitions
  void NewDirection(Wall wall);
  void NewWalk();
  void NewShake();
  // Helper Functions
  void DetectWall();
  TargetVector GetTargetVector(int x_start, int y_start, int x_end, int y_end);
  void ConvertToTurtleVector(TargetVector *target);
  utilities::Coordinate GetTargetCoordinate(int x, int y, int dir, int dist);

  // Constants
  const int grid_width;
  const int grid_height;

  // Constant parameters
  const int size{8};
  const int shakes{5};
  const int sleeepcycle{20};
  const int walking_step{1};
  const int rotation_step{5};

  // interal state variable
  State state = State::Moving;
  Instruction curr_instruction;
  bool alive{true};
  int head_x;
  int head_y;
  int rotation = 0;
  int stepping_speed = 3;

  // counters and targets
  int steps{0};
  int sleep_counter{0};

  std::deque<Instruction> motion_path;

};

#endif //TURTLE_H