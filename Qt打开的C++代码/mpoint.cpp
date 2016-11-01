#include "mpoint.h"

double* MPoint::dump() {
    double *temp = new double[4];
    temp[0] = x;
    temp[1] = y;
    temp[2] = z;
    temp[3] = stable;
    return temp;
}

void MPoint::set(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}
