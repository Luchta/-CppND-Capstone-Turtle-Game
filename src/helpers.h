#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"
 
namespace helper {

    class Coordinate {
    public:
        Coordinate(int x_, int y_) : x(x_), y(y_) {}
        int x;
        int y;
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

} // namespace helper

#endif /* HELPERS_H */