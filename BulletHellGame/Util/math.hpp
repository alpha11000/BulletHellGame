#pragma once

#include <cmath>
#include <iostream>

namespace math {
	const double pi = std::acos(-1);

	struct Vector3 {
		float v[3];

		Vector3(float x = 0, float y = 0, float z = 0);
		float& operator[](int index);
		float operator[](int index) const;
		Vector3 operator-(const Vector3& p);
		Vector3 operator-(const Vector3& p) const;
		Vector3 operator+(const Vector3& p);
		Vector3 operator+(const Vector3& p) const;
		Vector3 operator/(const float div);
		Vector3 operator/(const float div) const;
		Vector3 operator*(const float mul);
		Vector3 operator*(const float mul) const;
		friend bool operator==(const Vector3& lhs, const Vector3& rhs);
		friend std::ostream& operator<<(std::ostream& out, const Vector3& p);
	};
	
	inline float euclidean(Vector3 x1, Vector3 x2 = Vector3()) { 
		return 
			std::sqrt(
				(x1[0] - x2[0]) * (x1[0] - x2[0]) +
				(x1[1] - x2[1]) * (x1[1] - x2[1]) +
				(x1[2] - x2[2]) * (x1[2] - x2[2])
			);
	};

	inline int manhattan(Vector3 x1, Vector3 x2 = Vector3()) {
		return (int)
			std::abs(x1.v[0] - x2.v[0]) +
			std::abs(x1.v[1] - x2.v[1]) +
			std::abs(x1.v[2] - x2.v[2]);
	}
}
