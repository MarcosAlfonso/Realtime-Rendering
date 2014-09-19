#include <glm/glm.hpp>

#include <btBulletCollisionCommon.h>

class Helper
{
public:

	static btVector3 toBullet(const glm::vec3 &vec)
	{
		return btVector3(vec.x, vec.y, vec.z);
	}

	static glm::vec3 toGLM(const btVector3 &vec)
	{
		return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
	}

private:

};