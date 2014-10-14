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

extern btDiscreteDynamicsWorld *dynamicsWorld;

//Physics Component, added to any entity that needs physical interaction (and selection at the moment)
PhysicsComponent::PhysicsComponent(BaseEntity * parent, CollisionShapeEnum type, int _mass, std::vector<float> heights)
{
	Name = "Physics Component";

	//Set parent container
	parentEntity = parent;
	componentType = PHYSICS;


	//Set from the CollisionShapeEnum, figures out which kind collisionShape to create
	if (type == BOX)
	{
		mass = _mass;
		collisionShape = new btBoxShape(Helper::toBullet(parentEntity->Transform->scaleVec));
	}
	else if (type == SPHERE)
	{
		mass = _mass;
		collisionShape = new btSphereShape(parentEntity->Transform->scaleVec.x);
		collisionShape->setLocalScaling(Helper::toBullet(parentEntity->Transform->scaleVec));
	}
	else if (type == TERRAIN)
	{
		mass = 0;
		
		heightFieldArray = heights;
		
		collisionShape = new btHeightfieldTerrainShape(30, 30, &heightFieldArray[0], 1, -100, 100, 1, PHY_FLOAT, false);

		//Why is the scaling of this non-uniform?
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

	//UserPointer to itself, to make object selection work
	rigidBody->setUserPointer((void*)this);

	dynamicsWorld->addRigidBody(rigidBody);

}

PhysicsComponent::~PhysicsComponent()
{
	delete(collisionShape);
	delete(motionState);
	dynamicsWorld->removeRigidBody(rigidBody);
	delete(rigidBody);
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

	parentEntity->Transform->setPosition(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
}