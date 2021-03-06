#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

#include <string.h>

struct Vec3 {
	Vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {
	}

	float x, y, z;

	void normalize() {
		const float n = std::sqrt(x * x + y * y + z * z);
		assert(n > 0.0f);

		x /= n;
		y /= n;
		z /= n;
	}

	float dot(const Vec3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross(const Vec3& v) const {
		return Vec3(
		           y * v.z - z * v.y,
		           z * v.x - x * v.z,
		           x * v.y - y * v.x
		       );
	}

	Vec3 operator + (const Vec3& v) const {
		return Vec3(
		           x + v.x,
		           y + v.y,
		           z + v.z
		       );
	}

	Vec3 operator - (const Vec3& v) const {
		return Vec3(
		           x - v.x,
		           y - v.y,
		           z - v.z
		       );
	}

	Vec3 operator *(const float& s) const {
		return Vec3(
		           x * s,
		           y * s,
		           z * s
		       );
	}

	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

} __attribute__((aligned(16)));

typedef Vec3 Vertex;

struct Triangle {
	unsigned v0, v1, v2;
} __attribute__((aligned(32)));

struct Ray {
	Vec3 origin;
	Vec3 direction;
};

struct Mat4 {
	float m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	float* operator[](std::size_t index) {
		return m + index * 4;
	}

	const float* operator[](std::size_t index) const {
		return m + index * 4;
	}

	Mat4 operator*(const Mat4& b) const {
		Mat4 mult;
		memset(mult.m, 0, sizeof(float)*16);

		for(std::size_t i = 0; i < 4; ++i)
			for(std::size_t j = 0; j < 4; ++j)
				for(std::size_t k = 0; k < 4; ++k)
					mult[i][j] += (*this)[i][k] * b[k][j];

		return mult;
	}

	void transpose() {
		for(std::size_t i = 0; i < 4; ++i)
			for(std::size_t j = 0; j < 4; ++j)
				std::swap(m[i+j*4], m[i*4+j]);
	}
};

struct Camera {
	Vec3 target = Vec3(0, 0, 0);
	Vec3 position = Vec3(0, 0, -500);

	/// a silly version of "making rays" from screen coordinates (x and y are in -1..1)
	Ray makeRay(float x, float y) const {
		static const Vec3 s_up(0, 1, 0);

		Vec3 fwd = target - position;
		fwd.normalize();

		Vec3 side = s_up.cross(fwd);
		side.normalize();

		Vec3 up = fwd.cross(side);
		up.normalize();

		Vec3 dir = fwd + side * -x + up * y;
		dir.normalize();

		return Ray{
			position,
			dir
		};
	}

	void rotate(float xangle, float yangle) {
		Vec3 dir = target - position;

		float xrot = atan2(dir.z, dir.x);
		xrot += xangle;

		float yrot = atan2(dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));
		yrot += yangle;
		if(yrot < -M_PI/2.0 + 0.01)
			yrot = -M_PI/2.0 + 0.01;
		if(yrot > M_PI/2.0 - 0.01)
			yrot = M_PI/2.0 - 0.01;

		dir = Vec3(
		          cos(xrot) * cos(yrot),
		          sin(yrot),
		          sin(xrot) * cos(yrot)
		      ) * dir.length();

		position = target - dir;
	}
};
