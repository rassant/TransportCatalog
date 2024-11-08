#pragma once

#include <cmath> 
#include <limits>

namespace Detail
{

struct Coordinates {
    Coordinates (double x, double y):lat (x),lng (y){}
    double lat;
    double lng;
    bool operator == (const Coordinates& other) const {
        const auto epsilon = std::numeric_limits<double>::epsilon();

        return fabs (lat - other.lat) < epsilon && fabs (lng - other.lng) < epsilon;
        /*return lat == other.lat && lng == other.lng;*/
    }
    bool operator!=(const Coordinates& other) const {
        return !(* this == other);
    }
};

inline double 
ComputeDistance (Coordinates from, Coordinates to){
    using namespace std;
    if (from == to){
        return 0;
    }
    const int radius_earth = 6371000;
    static const double dr = 3.1415926535 / 180.;
    return acos (sin (from.lat * dr) * sin (to.lat * dr)
                + cos (from.lat * dr) * cos (to.lat * dr)
                * cos (abs (from.lng - to.lng) * dr)) * radius_earth;
}
}
