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
	PhysicsComponent(std::shared_ptr<BaseEntity> parent, CollisionShapeEnum type, int _mass, std::vector<float> heights);
	~PhysicsComponent();

	void Update();
	void TransformUpdate();
	void SetPosition(glm::vec3 trans);

	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btRigidBody> rigidBody;


private:
	int mass;
	std::vector<float> heightFieldArray;
	std::shared_ptr<btDefaultMotionState> motionState;

};
