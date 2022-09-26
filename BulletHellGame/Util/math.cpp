#include "math.hpp"

math::Vector3::Vector3(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

float& math::Vector3::operator[](int index) {
	return v[index];
}

float math::Vector3::operator[](int index) const {
	return v[index];
}

std::ostream& math::operator<<(std::ostream& out, const math::Vector3& v) {
	out << "(" << v[0] << "," << v[1] << "," << v[2] << ")";
	return out;
}

math::Vector3 math::Vector3::operator+(const math::Vector3& v1) {
	return Vector3(v[0] + v1[0], v[1] + v1[1], v[2] + v1[2]);
}

math::Vector3 math::Vector3::operator+(const math::Vector3& v1) const {
	return Vector3(v[0] + v1[0], v[1] + v1[1], v[2] + v1[2]);
}

math::Vector3 math::Vector3::operator-(const math::Vector3& v1) {
	return Vector3(v[0] - v1[0], v[1] - v1[1], v[2] - v1[2]);
}

math::Vector3 math::Vector3::operator-(const math::Vector3& v1) const {
	return Vector3(v[0] - v1[0], v[1] - v1[1], v[2] - v1[2]);
}

math::Vector3 math::Vector3::operator/(const float div) {
	return Vector3(v[0] / div, v[1] / div, v[2] / div);
}

math::Vector3 math::Vector3::operator/(const float div) const {
	return Vector3(v[0] / div, v[1] / div, v[2] / div);
}

math::Vector3 math::Vector3::operator*(const float mul) {
	return Vector3(v[0] * mul, v[1] * mul, v[2] * mul);
}

math::Vector3 math::Vector3::operator*(const float mul) const {
	return Vector3(v[0] * mul, v[1] * mul, v[2] * mul);
}

bool operator==(const math::Vector3& lhs, const math::Vector3& rhs) {
	return (lhs[0] == rhs[0] 
		 && lhs[1] == rhs[1]
		 && lhs[2] == lhs[2]
		);
}