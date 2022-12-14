#pragma once

#include <map>
#include "../Util/math.hpp"
#include "../Renderer/model.hpp"

namespace lgc {
	class rbAABB;
	class Ship;
	class Bullet;

	class Actor {
	protected:
		bool removeable;

		vis::GameObject* gameObject;
		std::map<std::string, vis::Material>* materials;
		math::Vector3 pos, rot;

	public:
		Actor(
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3()
		);

		void onUpdate();
		
		void draw();

		inline bool isRemoveable() {
			return removeable;
		}

		inline Actor& setPosition(float tX = 0, float tY = 0, float tZ = 0) {
			pos[0] = tX;
			pos[1] = tY;
			pos[2] = tZ;
			return *this;
		}

		inline Actor& setRotation(float rX = 0, float rY = 0, float rZ = 0) {
			rot[0] = rX;
			rot[1] = rY;
			rot[2] = rZ;
			return *this;
		}

		inline Actor& setGameObject(vis::GameObject* newGO) {
			gameObject = newGO;
			return *this;
		}

		inline Actor& setMaterials(vis::MTL *newMTL) { 
			materials = &(newMTL->mtlMaterials);
			return *this;
		}

		inline vis::GameObject& getGameObject() { return *gameObject; }
		inline std::map<std::string, vis::Material>& getMaterials() { return *materials; }
		inline math::Vector3 getPosition() { return pos; }
		inline math::Vector3 getRotation() { return rot; }
	};

	class Moveable : virtual public Actor {
	protected:
		math::Vector3 accel;
		math::Vector3 velocity;
		math::Vector3 maxVel;
		float maxVelMagnitude, spdLimit;
		bool isAccelerating;

		void updateVelocity();
		void move();

	public:
		Moveable(
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 max_vel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3()
		);

		void onUpdate();

		inline Moveable& setMaxVel(math::Vector3 newMaxVel) {
			maxVel = newMaxVel;
			maxVelMagnitude = math::euclidean(maxVel);
			return *this;
		}

		inline Moveable& changeAcceleration(math::Vector3 acc) {
			accel = accel + acc;
			return *this;
		}

		inline Moveable& setAcceleration(math::Vector3 acc) {
			accel = acc;
			return *this;
		}

		inline math::Vector3 getVelocity() { return velocity; }
		inline Moveable& setAccelerating(bool shouldAccelerate) { 
			isAccelerating = shouldAccelerate; 
			return *this;
		}
	};

	class Collidable : virtual public Actor {
	protected:
		rbAABB* shape;
		math::Vector3 min, max;

	public:
		Collidable(
			rbAABB* shape = nullptr,
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3()
		);

		inline Collidable& setHitbox(rbAABB* newShape) {
			shape = newShape;
			return *this;
		}

		void updateHitbox();
		void onUpdate();

		void _renderHitbox();
		inline math::Vector3 getMin() { return min; }
		inline math::Vector3 getMax() { return max; }

		inline virtual void onCollide(Ship& s) {};
		inline virtual void onCollide(Bullet& b) {};
	};

	class Shooter : virtual public Actor {
	protected:
		int shootRate, ticksCounter, bulletDamage;
		float shootDelayS;
		bool isShooting, isAlly;
		math::Vector3 bulletVel, bulletMaxVel, bulletAccel;
		vis::GameObject* bulletModel;
		rbAABB* bulletHitbox;
		vis::MTL* bulletMTL;

	public:
		Shooter(
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			vis::GameObject* bulletModel = nullptr,
			rbAABB* bulletHitBox = nullptr,
			float shootDelayS = 0, int fps = 0
		);

		void onUpdate();

		void shoot();

		inline Shooter& setShooting(bool shouldShoot) { 
			isShooting = shouldShoot;
			return *this;
		}

		inline Shooter& setBulletGameObject(vis::GameObject* bulletGO) {
			bulletModel = bulletGO;
			return *this;
		}

		inline Shooter& setBulletMaterial(vis::MTL* newBulletMTL) {
			bulletMTL = newBulletMTL;
			return *this;
		}

		inline Shooter& setBulletHitbox(rbAABB* newBulletHitbox) {
			bulletHitbox = newBulletHitbox;
			return *this;
		}

		inline Shooter& setBulletDamage(int newDamage) {
			bulletDamage = newDamage;
			return *this;
		}

		inline Shooter& setBulletVel(math::Vector3 newBulletVel) {
			bulletVel = newBulletVel;
			return *this;
		}

		inline Shooter& setBulletMaxVel(math::Vector3 newBulletMaxVel) {
			bulletMaxVel = newBulletMaxVel;
			return *this;
		}

		inline Shooter& setBulletAccel(math::Vector3 newBulletAcc) {
			bulletAccel = newBulletAcc;
			return *this;
		}

		inline Shooter& setShootDelay(float newShootDelaySeconds, int fps) {
			shootDelayS = newShootDelaySeconds;
			shootRate = newShootDelaySeconds * fps;
			return *this;
		}

		inline Shooter& setIsAlly(bool newIsAlly) {
			isAlly = newIsAlly;
			return *this;
		}

	};


	class Bullet : public Moveable, public Collidable {
	private:
		int damage;

	public:
		Bullet(
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 maxVel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3(),
			int damage = 0,
			rbAABB* shape = nullptr
		);

		void onUpdate();

		void onCollide(lgc::Bullet& b);
		void onCollide(lgc::Ship& s);

		inline Bullet& setDamage(int newDamage) {
			damage = newDamage;
			return *this;
		}

		inline int getDamage() { 
			return damage;
		}
	};

	class Ship : public Moveable, public Shooter, public Collidable {
	private:
		int hp, hp_buffer, score;
	public:
		Ship(
			vis::GameObject* gameObject = nullptr,
			math::Vector3 position = math::Vector3(),
			math::Vector3 rotation = math::Vector3(),
			math::Vector3 velocity = math::Vector3(),
			math::Vector3 maxVel = math::Vector3(),
			math::Vector3 acceleration = math::Vector3(),
			vis::GameObject* bulletModel = nullptr,
			rbAABB* bulletHitbox = nullptr,
			float shootDelayS = 0, int fps = 0, int hp = 0, int damage = 0,
			rbAABB* shape = nullptr
		);

		void onCollide(lgc::Bullet& b);
		void onCollide(lgc::Ship& s);

		void onDeath();
		
		inline int getHP() { return hp; }

		inline Ship& setHP(int newHP) {
			hp = newHP;
			return *this;
		}

		inline Ship& setScore(int newScore) {
			score = newScore;
			return *this;
		}

		void onUpdate();
	};

}