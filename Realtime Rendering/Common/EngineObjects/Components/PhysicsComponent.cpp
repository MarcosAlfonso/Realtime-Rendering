#pragma once
#include <iostream>
#include "PhysicsComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../GameEntity.h"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <exception>

//dynamicsWorld from main
extern btDiscreteDynamicsWorld* dynamicsWorld;

PhysicsComponent::PhysicsComponent(GameEntity* parent, CollisionShapeEnum type, const btVector3 &dims, int _mass, btScalar * terrainHeights )
{
	//Set parent container
	parentEntity = parent;

	btScalar mass;

	if (type == BOX)
	{
		mass = _mass;
		collisionShape = new btBoxShape(dims);
	}
	else if (type == SPHERE)
	{
		mass = _mass;
		btVector3 positions[1] = { btVector3(0, 0, 0)};
		btScalar radi[1] = { 1 };
		collisionShape = new btMultiSphereShape(positions, radi, 1);
		collisionShape->setLocalScaling(dims);
	}
	else if (type == TERRAIN)
	{
		mass = 0;
		collisionShape = new btHeightfieldTerrainShape(100, 100, terrainHeights, 1, 0, 400, 1, PHY_FLOAT, false);
	}

	//Motion state sets up initial position and rotation of physics, which is set to Transform
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(parentEntity->Transform->transVec.x, parentEntity->Transform->transVec.y, parentEntity->Transform->transVec.z)));

	//Mass calculations
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	
	//Rigid body creation
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, fallInertia);
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setMassProps(mass, fallInertia);

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
	parentEntity->Transform->setPosition(glm::vec3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));
	parentEntity->Transform->setRotation(glm::quat(transform.getRotation().getW(), transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ()));
}

void PhysicsComponent::SetPosition(glm::vec3 trans)
{
	btTransform transform = rigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(trans.x, trans.y, trans.z));
	rigidBody->setCenterOfMassTransform(transform);
}

void PhysicsComponent::toString()
{

}

