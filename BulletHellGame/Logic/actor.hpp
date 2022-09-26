#pragma once

#include <map>
#include "../Util/math.hpp"
#include "../Renderer/model.hpp"

namespace lgc {
	class Actor {
	protected:
		bool removeable;

		vis::GameObject gameObject;
		std::map<std::string, vis::Material> materials;
		math::Vector3 pos, rot;

	public:
		Actor(
			vis::GameObject gameObject = vis::GameObject(), 
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3()
		);

		void setMaterials(vis::MTL materials);
		void onUpdate();
		
		void draw();

		inline bool isRemoveable() {
			return removeable;
		}

		inline void setPosition(float tX = 0, float tY = 0, float tZ = 0) {
			pos[0] = tX;
			pos[1] = tY;
			pos[2] = tZ;
		}

		inline void setRotation(float rX = 0, float rY = 0, float rZ = 0) {
			rot[0] = rX;
			rot[1] = rY;
			rot[2] = rZ;
		}

		inline void setGameObject(vis::GameObject newGO) { gameObject = newGO; }
		inline vis::GameObject getGameObject() {return gameObject;}
		inline std::map<std::string, vis::Material> getMaterials() { return materials; }
		inline math::Vector3 getPosition() { return pos; }
		inline math::Vector3 getRotation() { return rot; }
	};

	class Moveable : virtual public Actor {
	protected:
		math::Vector3 accel;
		math::Vector3 velocity;
		math::Vector3 maxVel;
		float maxVelMagnitude;
		bool isAccelerating;
		float spdLimit;

		void updateVelocity();
		void move();

	public:
		Moveable(
			vis::GameObject gameObject = vis::GameObject(),
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 max_vel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3()
		);

		void onUpdate();

		inline void setMaxVel(float vX = 0, float vY = 0, float vZ = 0) {
			maxVel[0] = vX;
			maxVel[1] = vY;
			maxVel[2] = vZ;
			maxVelMagnitude = math::euclidean(maxVel);
		}

		inline void changeAcceleration(math::Vector3 acc) {
			accel = accel + acc;
		}

		inline void setAcceleration(float vX = 0, float vY = 0, float vZ = 0) {
			accel[0] = vX;
			accel[1] = vY;
			accel[2] = vZ;
		}

		inline math::Vector3 getVelocity() { return velocity; }
		inline void setAccelerating(bool shouldAccelerate) { isAccelerating = shouldAccelerate; }
	};

	class Shooter : virtual public Actor {
	protected:
		int shootRate, shootDelayS, damage, ticksCounter;
		bool isShooting;
		vis::GameObject bulletModel;

	public:
		Shooter(
			vis::GameObject gameObject = vis::GameObject(),
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			vis::GameObject bulletModel = vis::GameObject(),
			int shootDelayS = 0, int fps = 0, int damage = 0
		);

		void shoot(
			int damage,
			math::Vector3 vel = math::Vector3(),
			math::Vector3 max_vel = math::Vector3(),
			math::Vector3 accel = math::Vector3()
		);

		inline void setShooting(bool shouldShoot) { 
			if (shouldShoot != isShooting) ticksCounter = 0;
			isShooting = shouldShoot;
		}

	};

	class Ship;
	class Bullet;

	class Bullet : public Moveable {
	private:
		int damage;
	public:
		Bullet(
			int damage = 0,
			vis::GameObject gameObject = vis::GameObject(),
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 maxVel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3()
		);

		void onCollide(lgc::Ship other);

		inline int getDamage() { return damage; }
	};

	class Ship : public Moveable, public Shooter {
	private:
		int hp;
	public:
		Ship(
			vis::GameObject gameObject = vis::GameObject(),
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 maxVel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3(),
			vis::GameObject bulletModel = vis::GameObject(),
			int shootDelayS = 0, int fps = 0, int hp = 0, int damage = 0
		);

		void onCollide(lgc::Bullet other);
		void onCollide(lgc::Ship other);
		
		inline int getHP() { return hp; }

		void onUpdate();
	};

}