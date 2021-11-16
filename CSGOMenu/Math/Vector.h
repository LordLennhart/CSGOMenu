#ifndef VECTOR_H
#define VECTOR_H

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;

	Vec3 operator+(Vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator*(float d) {
		return{ x * d, y * d, z * d };
	}

};
struct Vec4 {
	float x, y, z, w;
};

#endif // !1

