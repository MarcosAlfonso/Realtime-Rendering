#pragma once
#include "BaseComponent.h"

#include <glm/glm.hpp>

#include <vector>

class BaseEntity;
class btRigidBody;
class btDefaultMotionState;
class btCollisionShape;

//Physics Types
enum CollisionShapeEnum{ BOX, SPHERE, TERRAIN };

//Physics Component, added to any entity that needs physical interaction (and selection at the moment)
class PhysicsComponent : public BaseComponent
{
public:
	PhysicsComponent(BaseEntity* parent, CollisionShapeEnum type, int _mass, std::vector<float> heights);
	~PhysicsComponent();

	void Update();
	void TransformUpdate();
	void SetPosition(glm::vec3 trans);

	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;


private:
	int mass;
	std::vector<float> heightFieldArray;
	btDefaultMotionState* motionState;

};
