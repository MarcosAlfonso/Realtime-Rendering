#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <GL\gl.h>
#include <glm/glm.hpp>

class bulletDebugDraw : public btIDebugDraw
{
public:
	
	static void drawLineTest(const btVector3& from, const btVector3& to, const btVector3& color);

private:

};