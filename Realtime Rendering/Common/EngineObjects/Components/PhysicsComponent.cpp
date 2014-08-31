#include <iostream>

#include "PhysicsComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GameEntity.h"

extern btDiscreteDynamicsWorld* dynamicsWorld;

PhysicsComponent::PhysicsComponent(GameEntity* parent)
{
	parentEntity = parent;
	collisionShape = new btSphereShape(1);

	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(parentEntity->Transform->transVec.x, parentEntity->Transform->transVec.y, parentEntity->Transform->transVec.z)));

	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);

	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody);

}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update()
{

	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	parentEntity->Transform->setPosition(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	std::cout << glm::degrees(trans.getRotation().getX()) << std::endl;
}

void PhysicsComponent::toString()
{

}

