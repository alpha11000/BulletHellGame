#include "actor.hpp"
#include "../Renderer/renderer.hpp"
#include "../Logic/logic.hpp"

lgc::Actor::Actor(
	vis::GameObject* gObj,
	math::Vector3 position,
	math::Vector3 rotation) :
	gameObject(gObj),
	pos(position),
	rot(rotation) {
	removeable = false;
	materials = &Renderer::getInstance().error_mats;
}

void lgc::Actor::onUpdate() {
	if (
		pos[2] > Renderer::getInstance().zmax + 5 ||
		pos[2] < -5
	) removeable = true;
}

void lgc::Actor::draw() {
	Renderer::getInstance().drawActor(this);
}

lgc::Moveable::Moveable(
	vis::GameObject* gameObject,
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


lgc::Collidable::Collidable(
	rbAABB* shape,
	vis::GameObject* gameObject,
	math::Vector3 position,
	math::Vector3 rotation
) :
	shape(shape), 
	Actor(gameObject, position, rotation) {}

void lgc::Collidable::_renderHitbox() {
	math::Vector3 min = shape->getMinDisp(), max = shape->getMaxDisp();
	float vertices[8][3] = {
		{ pos[0] + max[0], pos[1] + max[1], pos[2] + max[2] },
		{ pos[0] + max[0], pos[1] + max[1], pos[2] + min[2] },
		{ pos[0] + max[0], pos[1] + min[1], pos[2] + min[2] },
		{ pos[0] + min[0], pos[1] + min[1], pos[2] + min[2] },

		{ pos[0] + min[0], pos[1] + min[1], pos[2] + max[2] },
		{ pos[0] + min[0], pos[1] + max[1], pos[2] + max[2] },
		{ pos[0] + min[0], pos[1] + max[1], pos[2] + min[2] },
		{ pos[0] + max[0], pos[1] + min[1], pos[2] + max[2] },
	};

	int indices[12][2] = {
		{5,4},
		{5,0},

		{4,7},
		{7,0},

		{6,5},
		{1,0},

		{3,4},
		{2,7},

		{6,3},
		{6,1},

		{3,2},
		{2,1}
	};

	glBegin(GL_LINES);

	for (int i = 0; i < 12; i += 1) {
		glVertex3f(vertices[indices[i][0]][0], vertices[indices[i][0]][1], vertices[indices[i][0]][2]);
		glVertex3f(vertices[indices[i][1]][0], vertices[indices[i][1]][1], vertices[indices[i][1]][2]);
	}
	glEnd();
}

void lgc::Collidable::updateHitbox() {
	min = pos + shape->getMinDisp();
	max = pos + shape->getMaxDisp();
}

void lgc::Collidable::onUpdate() {
	updateHitbox();
	Actor::onUpdate();
}


lgc::Shooter::Shooter(
	vis::GameObject* gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	vis::GameObject* bulletModel,
	rbAABB* bulletHitbox,
	float shootDelayS, int fps
) : Actor(gameObject, position, rotation),
	shootDelayS(shootDelayS),
	bulletModel(bulletModel),
	bulletHitbox(bulletHitbox),
	ticksCounter(0), isShooting(false), bulletDamage(0)
{
	shootRate = shootDelayS * fps;
	bulletVel = bulletAccel = math::Vector3();
	bulletMaxVel = math::Vector3(1, 0, 0);
}

void lgc::Shooter::onUpdate() {
	shoot();
	Actor::onUpdate();
}

void lgc::Shooter::shoot() {
	ticksCounter++;
	if (!isShooting) return;
	if (ticksCounter >= shootRate) ticksCounter = 0;
	if (ticksCounter == 0) {
		Logic::getInstance().addBullet(new Bullet(
			bulletModel, pos, rot, math::Vector3(0, 0, bulletVel[2]), bulletMaxVel, math::Vector3(0, 0, bulletAccel[2]), bulletDamage, bulletHitbox
		), isAlly);
	}

}


lgc::Bullet::Bullet(
	vis::GameObject* gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	math::Vector3 velocity,
	math::Vector3 maxVel,
	math::Vector3 acceleration,
	int damage,
	rbAABB* shape
) : Actor(gameObject, position, rotation),
	Moveable(gameObject, position, rotation, velocity, maxVel, acceleration),
	damage(damage),
	Collidable(shape, gameObject, position, rotation) {
	setAccelerating(true);
}

void lgc::Bullet::onUpdate() {
	move();
	updateHitbox();
	Actor::onUpdate();
}

void lgc::Bullet::onCollide(lgc::Bullet& b) {}

void lgc::Bullet::onCollide(lgc::Ship& s) {
	removeable = true;
	std::cout << "bullet x ship\n";
}


lgc::Ship::Ship(
	vis::GameObject* gameObject,
	math::Vector3 position,
	math::Vector3 rotation,
	math::Vector3 velocity,
	math::Vector3 maxVel,
	math::Vector3 acceleration,
	vis::GameObject* bulletModel,
	rbAABB* bulletHitbox,
	float shootRate, int shootDelayS, int hp, int damage,
	rbAABB* shape
) : Actor(gameObject, position, rotation),
	Moveable(gameObject, position, rotation, velocity, maxVel, acceleration),
	Shooter(gameObject, position, rotation, bulletModel, bulletHitbox, shootRate, shootDelayS),
	hp(hp),
	Collidable(shape, gameObject, position, rotation) {
	setBulletMaxVel(maxVel);
}

void lgc::Ship::onCollide(lgc::Bullet& b) {
	hp_buffer -= b.getDamage();
	std::cout << "bullet x ship\n";
	b.onCollide(*this);
}

void lgc::Ship::onCollide(lgc::Ship& s) {
	hp_buffer -= s.getHP();
}

void lgc::Ship::onUpdate() {
	move();
	updateHitbox();
	shoot();
	hp += hp_buffer, hp_buffer = 0;
	if (hp <= 0) removeable = true;
	Actor::onUpdate();
}