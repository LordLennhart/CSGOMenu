#include "Math.h"

namespace math {
Vector3::Vector3() : x{0.f}, y{0.f}, z{0.f} {}
Vector3::Vector3(float x, float y, float z) : x{x}, y{y}, z{z} {}

std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
  os << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z;
  return os;
}

std::wostream& operator<<(std::wostream& os, const Vector3& vec) {
  os << L"X: " << vec.x << L" Y: " << vec.y << L" Z: " << vec.z;
  return os;
}
}  // namespace math