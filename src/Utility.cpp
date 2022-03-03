#include "Utility.h"

namespace PFG
{
	float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q)
	{
		float d = glm::dot((p - q), n);
		return d;
	}

	bool MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3 &ci)
	{
		float t;

		float d0 = DistanceToPlane(n, c0, q);
		float d1 = DistanceToPlane(n, c1, q);

		if (glm::abs(d0) <= r)
		{
			ci = c0;
			t = 0.0f;
			return true;
		}

		if (d0 > r && d1 < r)
		{
			t = (d0 - r) / (d0 - d1);

			ci = (1 - t)*c0 + t*c1;

			return true;
		}

		return false;
	}

	bool MovingSphereToPlaneCollision2(glm::vec3 n, glm::vec3 c0, glm::vec3 c1, glm::vec3 q, float r, glm::vec3 &ci)
	{
		float t;

		float d0 = DistanceToPlane(n, c0, q);
		float d1 = DistanceToPlane(n, c1, q);

		if (glm::abs(d0) <= r)
		{
			ci = c0;
			t = 0.0f;
			return true;
		}

		if (d0 > r && d1 < r)
		{
			t = (d0 - r) / (d0 - d1);

			ci = (1 - t)*c0 + t * c1;

			return true;
		}

		return false;
	}

	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3& cp, glm::vec3& n)
	{
		float d = glm::length(c0 - c1);

		if (d <= (r1 + r2))
		{
			n = glm::normalize(c0 - c1);
			cp = r1 * n;

			return true;
		}

		return false;
	}
}