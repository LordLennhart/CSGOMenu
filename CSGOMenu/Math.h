#ifndef MATH_H
#define MATH_H

#include <iostream>

namespace math {

class Vector3 {
 public:
  float x, y, z;
  Vector3();
  Vector3(float x, float y, float z);
  Vector3(const Vector3& other) = default;
  Vector3& operator=(const Vector3& other) = default;
  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }
  Vector3& operator+(Vector3 arg) {
    x += arg.x;
    y += arg.y;
    z += arg.z;

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
  friend std::wostream& operator<<(std::wostream& os, const Vector3& vec);
};
}  // namespace math

#endif MATH_H
