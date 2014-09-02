#include <iostream>

#include "PhysicsComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GameEntity.h"

//dynamicsWorld from main
extern btDiscreteDynamicsWorld* dynamicsWorld;



PhysicsComponent::PhysicsComponent(GameEntity* parent, CollisionShapeEnum type, const btVector3 &dims)
{
	//Set parent container
	parentEntity = parent;

	if (type == BOX)
	{
		collisionShape = new btBoxShape(dims);
	}
	else if (type == SPHERE)
	{
		//change to btMultiSphereShape to allow for non uniform scaling
		btVector3 positions[1] = { btVector3(0, 0, 0)};
		btScalar radi[1] = { .5 };
		collisionShape = new btMultiSphereShape(positions, radi, 1);
		collisionShape->setLocalScaling(dims);
	}
	else if (type == TERRAIN)
	{
		//use btHeightfieldTerrainShape and figure that shit out, BUT get rid of lib noise and use glm
	}

	//Motion state sets up initial position and rotation of physics, which is set to Transform
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(parentEntity->Transform->transVec.x, parentEntity->Transform->transVec.y, parentEntity->Transform->transVec.z)));

	//Mass calculations
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	
	//Rigid body creation
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setMassProps(mass, fallInertia);

}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update()
{
	//Gets rigid body Transform containing position and rotation 
	btTransform rigidBodyTrans;
	rigidBody->getMotionState()->getWorldTransform(rigidBodyTrans);
	
	//Sets Entity Transform to rigid body position and rotation
	parentEntity->Transform->setPosition(glm::vec3(rigidBodyTrans.getOrigin().getX(), rigidBodyTrans.getOrigin().getY(), rigidBodyTrans.getOrigin().getZ()));
	parentEntity->Transform->setRotation(glm::quat(rigidBodyTrans.getRotation().getW(), rigidBodyTrans.getRotation().getX(), rigidBodyTrans.getRotation().getY(), rigidBodyTrans.getRotation().getZ()));
}

void PhysicsComponent::toString()
{

}

