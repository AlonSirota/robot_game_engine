#ifndef QUATERNION_H
#define QUATERNION_H
#include <iostream>
#include "Point.h"
#include <cmath>
#include <GL/glut.h>

struct Quaternion {
  // Member variables
  double w, x, y, z;

  // Constructor
  Quaternion(double w = 1.0, double x = 0.0, double y = 0.0, double z = 0.0)
      : w(w), x(x), y(y), z(z) {}

  static Quaternion identity() { return Quaternion(1, 0, 0, 0); }

  Quaternion(double angle, Point axis) {
    w = std::cos(angle / 2);
    x = axis.x * std::sin(angle / 2);
    y = axis.y * std::sin(angle / 2);
    z = axis.z * std::sin(angle / 2);
  }

  // Quaternion multiplication
  Quaternion operator*(const Quaternion &q) const {
    return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
                      w * q.x + x * q.w + y * q.z - z * q.y,
                      w * q.y - x * q.z + y * q.w + z * q.x,
                      w * q.z + x * q.y - y * q.x + z * q.w);
  }

  // Scalar division
  Quaternion operator/(double scalar) const {
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
  }

  // Conjugate of the quaternion
  Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }

  // Magnitude (norm) of the quaternion
  double magnitude() const { return std::sqrt(w * w + x * x + y * y + z * z); }

  // Normalization of the quaternion
  Quaternion normalize() const {
    double mag = magnitude();
    return Quaternion(w / mag, x / mag, y / mag, z / mag);
  }

  // Inverse of the quaternion
  Quaternion inverse() const {
    return conjugate() / (magnitude() * magnitude());
  }

  Point rotatePoint(const Point &point) const {
    Quaternion p(0, point.x, point.y, point.z);
    Quaternion q = (*this) * p * inverse();
    return {q.x, q.y, q.z};
  }

  GLfloat *toMatrix() const {
    GLfloat *matrix = new GLfloat[16]; // TODO: this is a memory leak.
    matrix[0] = 1 - 2 * y * y - 2 * z * z;
    matrix[1] = 2 * x * y + 2 * z * w;
    matrix[2] = 2 * x * z - 2 * y * w;
    matrix[3] = 0;
    matrix[4] = 2 * x * y - 2 * z * w;
    matrix[5] = 1 - 2 * x * x - 2 * z * z;
    matrix[6] = 2 * y * z + 2 * x * w;
    matrix[7] = 0;
    matrix[8] = 2 * x * z + 2 * y * w;
    matrix[9] = 2 * y * z - 2 * x * w;
    matrix[10] = 1 - 2 * x * x - 2 * y * y;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
    return matrix;
  }

  // Display the quaternion
  void display() const {
    std::cout << "Quaternion: (" << w << ", " << x << ", " << y << ", " << z
              << ")\n";
  }
};
#endif
