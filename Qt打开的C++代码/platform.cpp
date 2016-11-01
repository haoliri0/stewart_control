#include "platform.h"
#include <qDebug>

platform::platform(int _x, int _y, int _z, int _a, int _b, int _c) :
    state(_x, _y, _z, _a, _b, _c)
{

}

void platform::rotateLeft(bool direct)
{
    state.a += 0.02 * (direct?1:-1);
    if(state.a > 0.3 || state.a < -0.3) {
        state.a = 0.3*(state.a>0?1:-1);
    }
}

void platform::rotateTurn(bool direct)
{
    state.c += 0.02 * (direct?1:-1);
    if(state.c > 0.3 || state.c < -0.3) {
        state.c = 0.3*(state.c>0?1:-1);
    }
}

void platform::rotateUp(bool direct)
{
    state.b += 0.02    * (direct?1:-1);
    if(state.b > 0.3 || state.b < -0.3) {
        state.b = 0.3*(state.b>0?1:-1);
    }
}

void platform::up(bool direct)
{
    state.z += 0.2 * (direct?1:-1);
}

void platform::left(bool direct)
{
    state.x -=  0.2 * (direct?1:-1);
}

void platform::forward(bool direct)
{
    state.y +=  0.2 * (direct?1:-1);
}

void platform::reSet()
{
    if(state.a > 0.01)
        state.a -= 0.005;
    else if(state.a < -0.01)state.a += 0.005;
    if(state.b > 0.01)
        state.b -= 0.005;
    else if(state.b < -0.01) state.b += 0.005;
    if(state.c > 0.01)
        state.c -= 0.005;
    else if(state.c < -0.01) state.c += 0.005;
}

PlState platform::dump()
{
    return state;
}

