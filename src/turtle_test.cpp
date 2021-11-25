#include <iostream>
#include <assert.h>
#include <tuple>

#define private public
#include "turtle.h"

#define GRIDWIDTH 200
#define GRIDHEIGHT 200

class TurtleTest : private Turtle {
public:
    TurtleTest() : Turtle(GRIDWIDTH, GRIDHEIGHT) {}

    void test_CheckForFood();
};

class DataPoint {
public:
    DataPoint( int food_x_, int food_y_, int start_rot_, int steps1_, int rot_ = 0, int steps2_ = 0) : 
        food_x(food_x_), food_y(food_y_), start_rot(start_rot_), steps1(steps1_), rot(rot_), steps2(steps2_) {}

    int food_x = 0;
    int food_y = 0;
    int start_rot = 0;
    int steps1 = 0;
    int rot = 0;
    int steps2 = 0;
};

void TurtleTest::test_CheckForFood() {
    head_x = 100;
    head_y = 100;
    rotation = 0;
    state = State::Turn;

    int size = motion_path.size();

    // simple straight lines test
    std::deque<DataPoint> test_set;
    test_set = {
        DataPoint(100, 50, 0, 50),
        DataPoint(150, 100, 90, 50),
        DataPoint(100, 150, 180, 50),
        DataPoint(50, 100, 270, 50),
        DataPoint(150, 50, 45, 71),
        DataPoint(150, 150, 135, 71),
        DataPoint(50, 150, 225, 71),
        DataPoint(50, 50, 315, 71),
    };

    int counter = 0;
    while (!test_set.empty())
    {
        counter++;
        state = State::Turn;
        DataPoint CurrP = test_set.front();
        rotation = CurrP.start_rot;

        CheckForFood(CurrP.food_x, CurrP.food_y);

        assert(motion_path.size() == 1);

        auto instruction = motion_path.front();
        assert(instruction.action_ == State::Walk);
        assert(instruction.steps_ == CurrP.steps1);
        motion_path.pop_front();

        test_set.pop_front();
        std::cout << "Test nr: " << counter << " passed!\n";
    }

    // complex path test
    std::deque<DataPoint> test_set2;
    test_set2 = {
        // 90 degreee dircetions
        DataPoint(110, 50, 0, 50, 90, 10),
        DataPoint(150, 90, 90, 50, 0, 10),
        DataPoint(150, 110, 90, 50, 180, 10),
        DataPoint(110, 150, 180, 50, 90, 10),
        DataPoint(90, 150, 180, 50, 270, 10),
        DataPoint(50, 110, 270, 50, 180, 10),
        DataPoint(50, 90, 270, 50, 0, 10),
        DataPoint(90, 50, 0, 50, 270, 10),
        // 45 degree directions
        DataPoint(140, 40, 45, 50, 315, 10),
        DataPoint(160, 60, 45, 50, 135, 10),
        DataPoint(160, 140, 135, 50, 45, 10),
        DataPoint(140, 160, 135, 50, 225, 10),
        DataPoint(60, 160, 225, 50, 135, 10),
        DataPoint(40, 140, 225, 50, 315, 10),
        DataPoint(40, 60, 315, 50, 225, 10),
        DataPoint(60, 40, 315, 50, 45, 10),
    };

    counter = 10;
    while (!test_set2.empty())
    {
        counter++;
        state = State::Turn;
        DataPoint CurrP = test_set2.front();
        rotation = CurrP.start_rot;

        CheckForFood(CurrP.food_x, CurrP.food_y);

        assert(motion_path.size() == 3);

        // first walk
        auto instruction = motion_path.front();
        assert(instruction.action_ == State::Walk);
        assert(instruction.steps_ == CurrP.steps1);
        motion_path.pop_front();
        
        // turn
        instruction = motion_path.front();
        assert(instruction.action_ == State::Turn);
        assert(instruction.steps_ == CurrP.rot);
        motion_path.pop_front();

        // second walk
        instruction = motion_path.front();
        assert(instruction.action_ == State::Walk);
        assert(instruction.steps_ == CurrP.steps2);
        motion_path.pop_front();

        test_set2.pop_front();
        std::cout << "Test nr: " << counter << " passed!\n";
    }

}

int
main()
{
    std::cout << "Tests Started!\n";
    TurtleTest turtletest;

    turtletest.test_CheckForFood();

    std::cout << "Tests Completed!\n";
    return 0;
}