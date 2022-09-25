#pragma once

#include <cmath>

namespace math {
	const double pi = std::acos(-1);

	struct Vector3 {
		float v[3];

		Vector3(float x = 0, float y = 0, float z = 0);

	};
	
	inline float euclidean(Vector3 x1, Vector3 x2 = Vector3()) { 
		return 
			std::sqrt(
			(x1.v[0] * x2.v[0]) +
			(x1.v[1] * x2.v[1]) +
			(x1.v[2] * x1.v[2])
		);
	};

	inline int manhattan(Vector3 x1, Vector3 x2 = Vector3()) {
		return
			std::abs(x1.v[0] - x2.v[0]) +
			std::abs(x1.v[1] - x2.v[1]) +
			std::abs(x1.v[2] - x2.v[2]);
	}
}
