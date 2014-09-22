#pragma once
#include "PhysicsComponent.h"

#include "../Entities/BaseEntity.h"
#include "../Util/helpers.cpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <iostream>

//dynamicsWorld from main
extern btDiscreteDynamicsWorld* dynamicsWorld;

PhysicsComponent::PhysicsComponent(BaseEntity* parent, CollisionShapeEnum type, int _mass, std::vector<float> heights )
{
	//Set parent container
	parentEntity = parent;

	if (type == BOX)
	{
		mass = _mass;
		collisionShape = new btBoxShape(Helper::toBullet(parentEntity->Transform->scaleVec));
	}
	else if (type == SPHERE)
	{
		mass = _mass;
		btVector3 positions[1] = { btVector3(0, 0, 0)};
		btScalar radi[1] = { 1 };
		collisionShape = new btMultiSphereShape(positions, radi, 1);
		collisionShape->setLocalScaling(Helper::toBullet(parentEntity->Transform->scaleVec));
	}
	else if (type == TERRAIN)
	{
		mass = 0;
		
		heightFieldArray = heights;
		
		collisionShape = new btHeightfieldTerrainShape(30, 30, &heightFieldArray[0], 1, -100, 100, 1, PHY_FLOAT, false);
		collisionShape->setLocalScaling(btVector3(2,1,2));

	}

	

	//Motion state sets up initial position and rotation of physics, which is set to Transform
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), Helper::toBullet(parentEntity->Transform->transVec)));

	//Mass calculations
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	
	//Rigid body creation
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setMassProps(mass, fallInertia);

	rigidBody->setUserPointer((void*)this);

	dynamicsWorld->addRigidBody(rigidBody);

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::Cleanup()
{
	delete(collisionShape);
	delete(motionState);

	dynamicsWorld->removeRigidBody(rigidBody);
	delete(rigidBody);
	delete(this);
}

void PhysicsComponent::Update()
{
	//Gets rigid body Transform containing position and rotation 
	btTransform transform = rigidBody->getCenterOfMassTransform();


	//Sets Entity Transform to rigid body position and rotation
	if (mass > 0)
	{
		parentEntity->Transform->setPosition(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
		parentEntity->Transform->setRotation(glm::quat(transform.getRotation().getW(), transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ()));
	}
}

void PhysicsComponent::SetPosition(glm::vec3 trans)
{
	btTransform transform = rigidBody->getCenterOfMassTransform();
	transform.setOrigin(Helper::toBullet(trans));
	rigidBody->setCenterOfMassTransform(transform);
}