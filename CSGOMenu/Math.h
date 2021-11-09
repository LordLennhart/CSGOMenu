#ifndef MATH_H
#define MATH_H


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
	};
}

#endif MATH_H
