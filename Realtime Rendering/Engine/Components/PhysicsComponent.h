#pragma once
#include "BaseComponent.h"

#include <glm/glm.hpp>

#include <vector>

class BaseEntity;
class btRigidBody;
class btDefaultMotionState;
class btCollisionShape;

typedef unsigned char byte_t;

//Physics Types
enum CollisionShapeEnum{ BOX, SPHERE, TERRAIN };

class PhysicsComponent : public BaseComponent
{
public:
	PhysicsComponent(BaseEntity* parent, CollisionShapeEnum type, int _mass, std::vector<float> heights);
	~PhysicsComponent();

	void Update();
	void toString();
	void Cleanup();
	void TransformUpdate();
	void SetPosition(glm::vec3 trans);

	btCollisionShape* collisionShape;



private:
	int mass;
	std::vector<float> heightFieldArray;
	btRigidBody* rigidBody;
	btDefaultMotionState* motionState;

};
