#include "actor.hpp"
#include "../Renderer/renderer.hpp"
#include "../Logic/logic.hpp"

lgc::Actor::Actor(
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation) :
	gameObject(gameObject),
	pos(position),
	rot(rotation) {
	removeable = false;
}

void lgc::Actor::onUpdate() {
	//if (pos[2] < -10) removeable = true;
}

void lgc::Actor::draw() {
	Renderer::getInstance().drawActor(*this);
}

lgc::Moveable::Moveable(
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	math::Vector3 velocity,
	math::Vector3 maxVel,
	math::Vector3 acceleration
) : Actor(gameObject, position, rotation),
	velocity(velocity),
	maxVel(maxVel),
	maxVelMagnitude(math::euclidean(maxVel)),
	accel(acceleration),
	isAccelerating(false),
	spdLimit(0.05) {}

void lgc::Moveable::updateVelocity() {
	if (math::euclidean(velocity) < maxVelMagnitude
		&& isAccelerating) {
		velocity = velocity + accel;
	}
	else {
		if (!isAccelerating &&
			(velocity[0] != 0 ||
			velocity[1] != 0 ||
			velocity[2] != 0))
		{
			if (std::abs(velocity[0]) >= spdLimit ||
				std::abs(velocity[1]) >= spdLimit ||
				std::abs(velocity[2]) >= spdLimit)
			{
				velocity[0] -= velocity[0] / 6;
				velocity[1] -= velocity[1] / 6;
				velocity[2] -= velocity[2] / 6;
			}
			else
				velocity = math::Vector3();
		}
	}
}

void lgc::Moveable::move() {
	updateVelocity();
	pos = pos + velocity;
}

void lgc::Moveable::onUpdate() {
	move();
	Actor::onUpdate();
}


lgc::Shooter::Shooter(
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	vis::GameObject bulletModel,
	float shootDelayS, int fps
) : Actor(gameObject, position, rotation),
	shootDelayS(shootDelayS),
	bulletModel(bulletModel),
	ticksCounter(0), isShooting(false), bulletDamage(0)
{
	shootRate = (float)shootDelayS * fps;
	bulletVel = bulletAccel = math::Vector3();
	bulletMaxVel = math::Vector3(1, 0, 0);
}

void lgc::Shooter::onUpdate() {
	shoot();
	Actor::onUpdate();
}

void lgc::Shooter::shoot() {
	if (!isShooting) return;
	if (ticksCounter == 0) {
		Logic::getInstance().addBullet(Bullet(
			bulletDamage, bulletModel, pos, rot, math::Vector3(0, 0, bulletVel[2]), bulletMaxVel, math::Vector3(0, 0, bulletAccel[2])
		));
	}
	ticksCounter++;

	if (ticksCounter >= shootRate) ticksCounter = 0;
}


lgc::Bullet::Bullet(
	int damage,
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	math::Vector3 velocity,
	math::Vector3 maxVel,
	math::Vector3 acceleration
) : Actor(gameObject, position, rotation),
	Moveable(gameObject, position, rotation, velocity, maxVel, acceleration),
	damage(damage) {
	setAccelerating(true);
}

void lgc::Bullet::onCollide(lgc::Ship other) {
	removeable = true;
}


lgc::Ship::Ship(
	vis::GameObject gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	math::Vector3 velocity,
	math::Vector3 maxVel,
	math::Vector3 acceleration,
	vis::GameObject bulletModel,
	float shootRate, int shootDelayS, int hp, int damage
) : Actor(gameObject, position, rotation),
Moveable(gameObject, position, rotation, velocity, maxVel, acceleration),
Shooter(gameObject, position, rotation, bulletModel, shootRate, shootDelayS),
hp(hp) {
	setBulletMaxVel(maxVel);
}

void lgc::Ship::onCollide(lgc::Bullet other) {
	hp -= other.getDamage();
	other.onCollide(*this);
}

void lgc::Ship::onCollide(lgc::Ship other) {
	hp -= other.getHP();
}

void lgc::Ship::onUpdate() {
	move();
	shoot();
	Actor::onUpdate();
}