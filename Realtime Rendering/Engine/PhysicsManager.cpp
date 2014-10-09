#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Graphics/bulletDebugDraw.h"

//Physics
btDiscreteDynamicsWorld* dynamicsWorld;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
bulletDebugDraw* drawer;

extern float DeltaTime;

//Physics Manager, handles the Bullet Physics sytem
void InitializePhysics()
{

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	//Creates Bullet world with the stuff referenced 
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	//Debug drawing options here
	drawer = new bulletDebugDraw();

	dynamicsWorld->setDebugDrawer(drawer); 
	dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void UpdatePhysics()
{
	dynamicsWorld->stepSimulation(DeltaTime);
}

void CleanupPhysics()
{
	delete(collisionConfiguration);
	delete(dispatcher);
	delete(overlappingPairCache);
	delete(solver);
	
	//This is commented because it now causes a crash, but it needs to happen im thinking
	delete(dynamicsWorld);
}