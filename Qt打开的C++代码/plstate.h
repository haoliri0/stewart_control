#ifndef PLSTATE
#define PLSTATE

class PlState { //平面状态类
public:
    PlState(int _x, int _y, int _z, int _a, int _b, int _c):x(_x), y(_y), z(_z),
        a(_a), b(_b), c(_c) {}

    double x, y, z;
    double a, b, c;
};

#endif // PLSTATE

