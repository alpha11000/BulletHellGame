#include "actor.hpp"
#include "../Renderer/renderer.hpp"

lgc::Actor::Actor(
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation) :
	gameObject(gameObject),
	pos(position),
	rot(rotation) {}

void lgc::Actor::setMatrials(vis::MTL materials) {
	this->materials = materials.mtlMaterials;
}

void lgc::Actor::onUpdate() {
	translate(0, 0, -1);
	if (pos.v[2] < -100) {
		removeable = true;
	}
}

void lgc::Actor::draw() {
	Renderer::getInstance().drawActor(*this);
}
