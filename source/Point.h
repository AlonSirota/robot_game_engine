// Point.h
#ifndef POINT_H
#define POINT_H

struct Point {
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
  double x;
  double y;
  double z;

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
