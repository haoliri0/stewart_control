#ifndef PLATFORM_H
#define PLATFORM_H

#include "plstate.h"

class Controller;

class platform //平面类 state中的x,y,z代表平面中心的绝对坐标,a,b,c代表三个欧拉旋转角，a绕x轴，b绕y轴，c绕z轴
{
public:
    platform(int, int, int, int, int, int);
    void rotateLeft(bool);
    void rotateTurn(bool);
    void rotateUp(bool);
    void up(bool);
    void down();
    void left(bool);
    void forward(bool);
    void reSet();
    PlState dump();

private:
    PlState state;
    friend class Controller;
};

#endif // PLATFORM_H
