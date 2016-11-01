#ifndef COORDI
#define COORDI

class Coordi { //坐标类（和点类有些重复。。。可以删去）
public:
    void set(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    double x, y, z;
};

#endif // COORDI

