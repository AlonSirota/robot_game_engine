// Point.h
#ifndef POINT_H
#define POINT_H

#include <cmath>

struct Point {
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
  double x;
  double y;
  double z;

  Point normalized() {
    double length = std::sqrt(x * x + y * y + z * z);
    return {x / length, y / length, z / length};
  }

  Point operator+(const Point &other) {
    return {x + other.x, y + other.y, z + other.z};
  }

  Point operator+=(const Point &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Point operator-=(const Point &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Point operator-(const Point &other) {
    return {x - other.x, y - other.y, z - other.z};
  }

  Point operator*(double scalar) {
    return {x * scalar, y * scalar, z * scalar};
  }
};

#endif // POINT_H
