#include "turtle.h"
#include <cmath>
#include <iostream>
#include <functional>

#define PI 3.14159265

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
   
Turtle::Turtle(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        curr_instruction(Action::Walk, 10) {
  motion_path.push_back(curr_instruction);
}

void Turtle::Update() {
  static int update_counter = 0;
  update_counter++;
  if (update_counter >= stepping_speed)
  {
    update_counter = 0;
    // get latest instruction
    curr_instruction = motion_path.front();
    switch (curr_instruction.action_)
    {
      case Action::Sleep:
        Sleep();
        break;

      case Action::Turn:
        Turn();
        break;

      case Action::Walk:
        Walk();
        break;
      
      default:
        std::cout << "Invalid Instruction! \n";
    }

    if(curr_instruction.completed == true){
      // remove completed instruction from path
      motion_path.pop_front();
      // if complete walk new way.
      if (motion_path.empty()){
        if (curr_instruction.action_ == Action::Turn){
          NewWalk();
        }
        else
        {
          NewDirection(Wall::None);
        }
      }
    }
  }
}

// TODO FOOD
void Turtle::CheckForFood(int x, int y){
  if (state == State::Moving){

    TargetVector target =  GetTargetVector(head_x, head_y, x, y);

    // create view angle of turtle and check for food in view
    int range = 15;
    int max_range = rotation + range;
    int min_range = rotation - range;
    bool in_range = false;
    if (min_range < 0 || max_range > 359)
    {
      min_range = min_range + 90;
      max_range = max_range + 90;
      int target_dir_90 = target.dir + 90;
      if (target_dir_90 > 360){
        target_dir_90 = target_dir_90 - 360;
      }
      in_range = (target_dir_90 <= max_range && target_dir_90 >= min_range);
    }else{
      in_range = (target.dir <= max_range && target.dir >= min_range);
    }
    // if food in view create instrutctions to walk to it
    if (in_range) {
      std::cout << "Found Food! \n";

      // Motion Planner:
      // convert to steps and directions for turtle
      ConvertToTurtleVector(&target);

      motion_path.push_back( Instruction(Action::Turn, target.dir) );
      motion_path.push_back( Instruction(Action::Walk, target.dist) );

      // get new position
      utilities::Coordinate targetXY = GetTargetCoordinate( head_x, head_y, target.dir, target.dist);

      // iterate untill close to food
      int distance = target.dist;
      while (distance >= size/2)
      {
        // set next walking path
        TargetVector new_target =  GetTargetVector(targetXY.x, targetXY.y, x, y);
        ConvertToTurtleVector(&new_target);
        motion_path.push_back( Instruction(Action::Turn, new_target.dir) );
        motion_path.push_back( Instruction(Action::Walk, new_target.dist) );
        // get new position
        targetXY = GetTargetCoordinate( targetXY.x, targetXY.y, new_target.dir, new_target.dist);
        distance = new_target.dist;
      }

      // set state
      state = State::Feeding;
      steps = 0;
    }
  }
}

void Turtle::Poke(){
  state = State::Shaking;
  stepping_speed--;
  if (stepping_speed <= 0){
    stepping_speed = 1;
  }
  NewShake();
}

bool Turtle::TurtleCell(int x, int y) {
  if ((x >= head_x) && (x <= (head_x + size)) && ((y >= head_y) && (y <= (head_y + size)))){
    return true;
  }
  return false;
}

/* Getters and Setters ********************************************************/

 bool Turtle::GetAlive() const{
   return alive;
 }

 utilities::Coordinate Turtle::GetHead() const{
   utilities::Coordinate Head(head_x, head_y, rotation);
   return Head;
 }

 int Turtle::GetSize() const{
   return size;
 }

 void Turtle::SetSpeed(int refresh_rate){
   stepping_speed = refresh_rate;
 }
 
void Turtle::SetAlive(bool state){
  alive = state;
}

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/* State Functions ************************************************************/
void Turtle::Walk(){
  UpdateHead();
  // check if walked enough
  if(steps >= curr_instruction.steps_){
    curr_instruction.completed = true;
    steps = 0;
  }
  else
  {
    DetectWall();
  }
}

void Turtle::Turn(){
    UpdateRotation();
    if(rotation == curr_instruction.steps_){
      curr_instruction.completed = true;
    }
}

void Turtle::Sleep(){
  sleep_counter++;
  if (sleep_counter >= curr_instruction.steps_)
  {
    curr_instruction.completed = true;
    sleep_counter = 0;
  }
}

/* Update Functions ***********************************************************/

/* update rotation while turning */
void Turtle::UpdateRotation() {
  // decide to turn left or right.
  int diff = curr_instruction.steps_ - rotation;
  if(diff > 180 || (diff > -180 && diff < 0))
  {
    rotation = rotation - rotation_step;
  }
  else
  {
    rotation = rotation + rotation_step;
  }
  // keep rotation below 360
  if (rotation >= 360){
    rotation = rotation - 360;
  } else if (rotation < 0){
    rotation = rotation + 360;
  }
}

/* Update Head position while walking */
void Turtle::UpdateHead() {
  switch (rotation) {
    case 0:
      head_y -= walking_step;
      break;

    case 45:
      head_x += walking_step;
      head_y -= walking_step;
      break;

    case 90:
      head_x += walking_step;
      break;

    case 135:
      head_x += walking_step;
      head_y += walking_step;
      break;

    case 180:
      head_y += walking_step;
      break;

    case 225:
      head_x -= walking_step;
      head_y += walking_step;
      break;

    case 270:
      head_x -= walking_step;
      break;

    case 315:
      head_x -= walking_step;
      head_y -= walking_step;
      break;
  }
  steps++;
}

/* check for wall in the current sourroundings*/
void Turtle::DetectWall() {
  bool wall = false;
  int security_dist = size;
  Wall wall_dir = Wall::None;

  // Right wall
  if (head_x >= grid_width - security_dist)
  {
    head_x = grid_width - security_dist;
    if (rotation == 90 || rotation == 135 || rotation == 45){
      wall = true;
      wall_dir = Wall::Right;
    }
  }
  // Left wall
  if (head_x <= 0){
    head_x = 0;
    if (rotation == 270 || rotation == 315 || rotation == 225){
      wall = true;
      wall_dir = Wall::Left;
    }
  }
  // Bottom Wall
  if (head_y >= grid_width -security_dist){
    head_y = grid_width - security_dist;
    if (rotation == 180 || rotation == 225 || rotation == 135){
      wall = true;
      wall_dir = Wall::Bottom;
    }
  }
  // Top Wall
  if (head_y <= 0){
    head_y = 0;
    if (rotation == 0 || rotation == 315 || rotation == 45){
      wall = true;
      wall_dir = Wall::Top;
    }
  }
  // Rotation change
  if (wall){
    motion_path.clear();
    // new direction
    NewDirection(wall_dir);
    wall = false;
    std::cout << "Found a Wall!\n";
  }
}

/* State Transitions **********************************************************/

/* Create shaking motion */
void Turtle::NewShake() {
  int new_rotation = 0;
  static bool left = false;
  motion_path.clear();

  for (int i = 0; i <= shakes; i++)
  {
    if(left)
    {
      new_rotation = rotation - 45;
      if(new_rotation < 0){
        new_rotation = new_rotation + 360;
      }
      left = false;
    }
    else
    {
      new_rotation = rotation + 45;
      if(new_rotation >= 360){
        new_rotation = new_rotation - 360;
      }

      left = true;
    }
    motion_path.push_back( Instruction(Action::Turn, new_rotation) );
  }
}

/* Set new random walking distance */
void Turtle::NewWalk(){
  std::random_device r;
  std::mt19937 e2(r());
  std::uniform_int_distribution<int> uniform_dist(10, grid_width/2);
  int distance = uniform_dist(e2);

  motion_path.push_back( Instruction(Action::Walk, distance) );
  state = State::Moving;

}

/* set new random walking direction*/
void Turtle::NewDirection(Wall wall){
  std::random_device r;
  bool clear = false;
  int new_dir = 0;

  // Choose a random mean between 1 and 6
  std::mt19937 e2(r());
  std::uniform_int_distribution<int> uniform_dist(0, 6);

  // avoid wall if present
  while(!clear){
    // create direction
    int mean = uniform_dist(e2);
    new_dir = (45 * mean);
    if(new_dir >= 360){
      new_dir = new_dir - 360;
    }

    // avoid wall
    switch (wall)
    {
      case Wall::Top :
        if(new_dir > 45 && new_dir < 315){
          clear = true;
        }
        break;

      case Wall::Right :
        if(new_dir > 135 || new_dir < 45){
          clear = true;
        }
        break;

      case Wall::Bottom :
        if(new_dir > 225 || new_dir < 135){
          clear = true;
        }
        break;

      case Wall::Left :
        if(new_dir > 315 || new_dir < 225){
          clear = true;
        }
        break;
    
      default:
        clear = true;
        break;
    }
  }

  motion_path.push_back( Instruction(Action::Turn, new_dir) );
  state = State::Moving;

}

/* Helper Functions ***********************************************************/

/* get direction and distance from two points */
Turtle::TargetVector Turtle::GetTargetVector(int x_start, int y_start, int x_end, int y_end){
    // definition of normal up
    int x1 = 0;
    int y1 = -1;
    // get direction to target
    int x2 = x_end - x_start;
    int y2 = y_end - y_start;
    int dot = x1 * x2 + y1 * y2;
    int det = x1 * y2 - x2 * y1;
    float dir_rad = atan2(det, dot);
    // convert to degree
    float dir_deg = dir_rad * (180/3.1415);
    int dir_deg_rnd = static_cast<int>(round(dir_deg));
    // address negative directions.
    if (dir_deg_rnd < 0){
      dir_deg_rnd = dir_deg_rnd + 360;
    }

    // distance
    float dist = sqrt(pow(x_end - x_start, 2) + pow(y_end - y_start, 2) * 1.0);
    int dist_rnd = static_cast<int>(round(dist));

    return TargetVector(dir_deg_rnd, dist_rnd);
}

/* convert to turtle grid based directions and distances */
void Turtle::ConvertToTurtleVector(TargetVector *target){
    // get nearest walkable rotation
    int rot_steps = nearbyint(static_cast<float>(target->dir) / 45.0);
    // protect from larger than 360 deg
    if (rot_steps >= 8){
      rot_steps = 0;
    }
    // Write in 45 deg steps
    target->dir = rot_steps * 45;

    // check for diagonal walking direction
    if(rot_steps%2 != 0){
      target->dist = target->dist / sqrt(2);
    }
}

/* get end point of given path */
utilities::Coordinate Turtle::GetTargetCoordinate(int x, int y, int dir, int dist){
    int x2 = 0;
    int y2 = 0;
    // x is horitonal top
    // y is vertical left
    // diagonal steps are one unit of dist each as well

    switch (dir) {
    case 0:
      x2 = x;
      y2 = y - dist;
      break;

    case 45:
      x2 = x + dist;
      y2 = y - dist;
      break;

    case 90:
      x2 = x + dist;
      y2 = y;
      break;

    case 135:
      x2 = x + dist;
      y2 = y + dist;
      break;

    case 180:
      x2 = x;
      y2 = y + dist;
      break;

    case 225:
      x2 = x - dist;
      y2 = y + dist;
      break;

    case 270:
      x2 = x - dist;
      y2 = y;      break;

    case 315:
      x2 = x - dist;
      y2 = y - dist;
      break;
    default:
      std::cout << "Invalid Function Call! -GetTargetXY- \n";
    }

    return utilities::Coordinate(x2,y2);
  }

// EOF