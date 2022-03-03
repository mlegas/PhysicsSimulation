#ifndef __PLANE__
#define __PLANE__

#include "GameObject.h"

class Plane : public GameObject
{
public:
	Plane();
	glm::vec3 GetNormal() { return _normal; }
	float GetBounciness() { return _bounciness; }
	void SetBounciness(float bounciness) { _bounciness = bounciness; }
	void SetRotation(float rotX, float rotY, float rotZ);
	float GetHalfOfSide() { return _halfOfSide; }
private:
	float _halfOfSide;
	glm::vec3 _normal;
	float _bounciness;
};

#endif