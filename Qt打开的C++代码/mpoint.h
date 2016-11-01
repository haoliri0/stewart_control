#ifndef MPOINT_H
#define MPOINT_H

class MPoint //点类
{
public:
    MPoint(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
        stable = 1;
    }

    MPoint() {
        x = y = z = 0;
        stable = 1;
    }

    void set(double, double, double);
    double* dump();
private:
    double x, y, z, stable;
};

#endif // MPOINT_H
