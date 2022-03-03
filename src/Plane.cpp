#include "Plane.h"
#include <glm/ext.hpp>

Plane::Plane()
{
	_bounciness = 0.0f;
	_normal = glm::vec3(0, -1, 0);

	_halfOfSide = 5.0f * _scale.x;
}

void Plane::SetRotation(float rotX, float rotY, float rotZ)
{
	_rotation.x = rotX; _rotation.y = rotY; _rotation.z = rotZ;

	_normal = glm::rotate(_normal, _rotation.x, glm::vec3(1, 0, 0));
	_normal = glm::rotate(_normal, _rotation.y, glm::vec3(0, 1, 0));
	_normal = glm::rotate(_normal, _rotation.z, glm::vec3(0, 0, 1));

}