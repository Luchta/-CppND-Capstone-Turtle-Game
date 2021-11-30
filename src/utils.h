#ifndef UTILS_H
#define UTILS_H

#include "SDL.h"
 
namespace utilities {

    class Coordinate {
    public:
        Coordinate(int x_, int y_, int rot_=0) : x(x_), y(y_), rot(rot_) {}
        int x;
        int y;
        int rot;
    };

    class Click_Message {
    public:
        Click_Message() : coord(0,0) {}
        Coordinate  coord;
        bool active = false;
    };

    class Food {
    public:
        Food() {}
        SDL_Point point;
        bool active = false;
    };

} // namespace utilities

#endif /* UTILS_H */