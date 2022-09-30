#pragma once
#include "../Util/math.hpp"

namespace lgc {
	class rigidBodyDesc {
	protected:
		math::Vector3 center;

		inline rigidBodyDesc(math::Vector3 center) :
			center(center) {}
	};

	class rbSphere : protected rigidBodyDesc {
		float radius;
	
	public:
		inline rbSphere(float radius = 0, math::Vector3 center = math::Vector3()) :
			radius(radius), rigidBodyDesc(center) {}

	};

	class rbAABB : protected rigidBodyDesc {
		math::Vector3 bounds;

	public:
		inline rbAABB(float dx, float dy, float dz, math::Vector3 center = math::Vector3()) :
			bounds(math::Vector3(dx, dy, dz)), rigidBodyDesc(center) {}

	};

	class collisionSolver {

	};
}

