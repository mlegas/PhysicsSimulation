#ifndef _Utility_H_
#define _Utility_H_

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace PFG
{
	float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q);

	bool MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci);

	bool MovingSphereToPlaneCollision2(glm::vec3 n, glm::vec3 c0, glm::vec3 c1, glm::vec3 q, float r, glm::vec3 &ci);

	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3& cp, glm::vec3& n);
}


#endif