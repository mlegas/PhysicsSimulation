#include "DynamicObject.h"
#include "Utility.h"
#include "Plane.h"
#include <iostream>
#include <glm/ext.hpp>

DynamicObject::DynamicObject()
{
	_friction = 0.7f;
	_freefall = glm::vec3(0, -9.81f, 0);
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
	// Set default value
	_bRadius = 0.5f;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_mass = 1.0f;
	_start = false;

	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	
	_torque = glm::vec3(0.0f, 0.0f, 0.0f);
	_angular_momentum = glm::vec3(0.0f, 0.0f, 0.0f);
	_R = glm::mat3(1.0f);
	_quatR = glm::quat(1, 0, 0, 0);

	float value = 2.0f / 5.0f *_mass *_bRadius*_bRadius;

	glm::mat3 temp = glm::mat3(value, 0.0f, 0.0f,
		0.0f, value, 0.0f,
		0.0f, 0.0f, value);

	_body_inertia_tensor_inverse = glm::inverse(temp);

	_inertia_tensor_inverse = _R * _body_inertia_tensor_inverse * glm::transpose(_R);

	_angular_velocity = _inertia_tensor_inverse * _angular_momentum;

}

DynamicObject::~DynamicObject()
{

}

void DynamicObject::Update(float deltaTs, std::vector<std::shared_ptr<DynamicObject>> _spheres,
	std::vector<std::shared_ptr<Plane>> _planes)
{
	if (_start == true)
	{
		ClearForces();
		ClearTorques();

		glm::vec3 f = ComputeForces();

		AddForce(f);

		CollisionResponses(deltaTs, _spheres, _planes);

		//RungeKutta2(deltaTs);
		RungeKutta4(deltaTs);
		//Euler(deltaTs);

	}
	UpdateModelMatrix();
}

void DynamicObject::RungeKutta2(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k0;
	glm::vec3 k1;
	glm::vec3 torque;
	glm::vec3 angM0;
	glm::vec3 angM1;
	glm::vec3 angV0;
	glm::vec3 angV1;
	
	torque = _torque;
	force = _force;
	acceleration = force * (1 /_mass);
	k0 = deltaTs * acceleration;
	angM0 = deltaTs * _torque;

	// change only angular momentum ok, calculate the rest after it

	torque = _torque + angM0 / 2.0f;
	force = _force + k0 / 2.0f;
	acceleration = force * (1 / _mass);
	k1 = deltaTs * acceleration;
	angM1 = deltaTs * _torque;

	_angular_momentum += angM1;

	ComputeInverseInertiaTensor();

	_angular_velocity = _inertia_tensor_inverse * _angular_momentum;

	glm::mat3 _omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += _omega_star * _R * deltaTs;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_R[i][j] > 1.0f || std::isnan(_R[i][j]) || std::isinf(_R[i][j]))
			{
				_R[i][j] = 1.0f;
			}
		}
	}

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);

	_velocity += k1;
	_position += _velocity * deltaTs;
}

void DynamicObject::RungeKutta4(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 torque;
	glm::vec3 k0;
	glm::vec3 k1;
	glm::vec3 k2;
	glm::vec3 k3;
	glm::vec3 angM0;
	glm::vec3 angM1;
	glm::vec3 angM2;
	glm::vec3 angM3;

	torque = _torque;
	force = _force;
	acceleration = force * (1 / _mass);
	k0 = deltaTs * acceleration;
	angM0 = deltaTs * torque;

	torque = _torque + angM0 / 2.0f;
	force = _force + k0 / 2.0f;
	acceleration = force * (1 / _mass);
	k1 = deltaTs * acceleration;
	angM1 = deltaTs * torque;

	torque = _torque + angM1 / 2.0f;
	force = _force + k1 / 2.0f;
	acceleration = force * (1 / _mass);
	k2 = deltaTs * acceleration;
	angM2 = deltaTs * torque;

	torque = _torque + angM2;
	force = _force + k2;
	acceleration = force * (1 / _mass);
	k3 = deltaTs * acceleration;
	angM3 = deltaTs * torque;

	_angular_momentum += (angM0 + 2.0f * angM1 + 2.0f * angM2 + angM3) / 6.0f;

	ComputeInverseInertiaTensor();

	_angular_velocity = _inertia_tensor_inverse * _angular_momentum;

	_omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += _omega_star * _R * deltaTs;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_R[i][j] > 1.0f || std::isnan(_R[i][j]) || std::isinf(_R[i][j]))
			{
				_R[i][j] = 1.0f;
			}
		}
	}

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);

	_velocity += (k0 + 2.0f * k1 + 2.0f * k2 + k3) / 6.0f;
	_position += _velocity * deltaTs;
}

glm::vec3 DynamicObject::ComputeForces()
{
	return _freefall * _mass;
}

void DynamicObject::ComputeInverseInertiaTensor()
{
	_inertia_tensor_inverse = _R * _body_inertia_tensor_inverse * glm::transpose(_R);
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_inertia_tensor_inverse[i][j] > 1.0f || std::isnan(_inertia_tensor_inverse[i][j]) || std::isinf(_inertia_tensor_inverse[i][j]))
			{
				_inertia_tensor_inverse[i][j] = 1.0f;
			}
		}
	}
}

void DynamicObject::Euler(float deltaTs)
{
	_velocity += (_force / _mass) * deltaTs;
	_position += _velocity * deltaTs;

	_angular_momentum += _torque * deltaTs;

	ComputeInverseInertiaTensor();

	_angular_velocity = _inertia_tensor_inverse * _angular_momentum;

	glm::mat3 _omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += _omega_star * _R * deltaTs;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_R[i][j] > 1.0f || std::isnan(_R[i][j]) || std::isinf(_R[i][j]))
			{
				_R[i][j] = 1.0f;
			}
		}
	}

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);
}

void DynamicObject::CollisionResponses(float deltaTs, std::vector<std::shared_ptr<DynamicObject>> _spheres,
	std::vector<std::shared_ptr<Plane>> _planes)
{
	glm::vec3 position = GetPosition();


	float bRadius = GetBoundingRadius();

	glm::vec3 velocity = _velocity;
	glm::vec3 contactPosition;
	glm::vec3 c1 = position + velocity * deltaTs;
	glm::vec3 c0 = position;


	float distance;

	for (int i = 0; i < _spheres.size(); i++)
	{
		if (_position != _spheres.at(i)->GetPosition())
		{
			distance = abs(position.x - _spheres.at(i)->GetPosition().x)
				+ abs(position.y - _spheres.at(i)->GetPosition().y)
				+ abs(position.z - _spheres.at(i)->GetPosition().z);

			if (distance < 2.0f)
			{
				glm::vec3 n;
				bool collision = PFG::SphereToSphereCollision(c0, c1, bRadius, _spheres.at(i)->GetBoundingRadius(), contactPosition, n);

				if (collision)
				{
					float impulseLinear = -(1 + 0.9)*glm::dot((_velocity - _spheres.at(i)->GetVelocity()), n) / (1 / _mass + 1 / _spheres.at(i)->GetMass());
					glm::vec3 impulseForce = impulseLinear * n / deltaTs;

					AddForce(impulseForce);

					glm::vec3 radius1 = contactPosition - position;
					glm::vec3 radius2 = contactPosition - _spheres.at(i)->GetPosition();

					float impulseAngular = -(1 + 0.9)*glm::dot((_velocity - _spheres.at(i)->GetVelocity()), n) *
						((glm::dot(n, glm::cross((glm::cross(radius1, n) * _body_inertia_tensor_inverse), radius1))) +
						(glm::dot(n, glm::cross((glm::cross(radius2, n) * _spheres.at(i)->GetBodyInertiaTensorInverse()), radius2))));
					
					impulseForce = impulseAngular * n / deltaTs;
					glm::vec3 torque = glm::cross(radius1, impulseForce);

					AddTorque(torque);

					torque = glm::cross(radius2, impulseForce);

					_spheres.at(i)->AddTorque(torque);
				}
			}
		}
	}

	for (int i = 0; i < _planes.size(); i++)
	{
		distance = abs(position.x - _planes.at(i)->GetPosition().x)
			+ abs(position.y - _planes.at(i)->GetPosition().y)
			+ abs(position.z - _planes.at(i)->GetPosition().z);

		if (distance <= _planes.at(i)->GetHalfOfSide())
		{
			bool collision = PFG::MovingSphereToPlaneCollision2(_planes.at(i)->GetNormal(), c0, c1, _planes.at(i)->GetPosition(), bRadius, contactPosition);

			if (collision)
			{
				float impulseLinear = (-(1 + _planes.at(i)->GetBounciness()) * glm::dot(_velocity, _planes.at(i)->GetNormal())) / (1 / _mass);
				glm::vec3 impulseForce = impulseLinear * _planes.at(i)->GetNormal() / deltaTs;
				AddForce(impulseForce + glm::vec3(0.0f, 9.8f*_mass, 0.0f));

				glm::vec3 radius = contactPosition - position;

				float impulseAngular = (-(1 + _planes.at(i)->GetBounciness()) * glm::dot(_velocity, _planes.at(i)->GetNormal()))
					* (glm::dot(_planes.at(i)->GetNormal(), glm::cross((glm::cross(radius, _planes.at(i)->GetNormal()) * _body_inertia_tensor_inverse), radius)));

				impulseForce = impulseAngular * _planes.at(i)->GetNormal() / deltaTs;
				glm::vec3 torque = glm::cross(radius, impulseForce);

				AddTorque(torque);

				ApplyFriction(deltaTs);
			}
		}
	}
}

void DynamicObject::ApplyFriction(float deltaTs)
{
	float frictionForce = _friction * _mass * _freefall.y * deltaTs;

	if (_velocity.x > 0)
	{
		if (_velocity.x < 0.01f)
		{
			_velocity.x = 0.0f;
		}
		else
		{
			AddForce(glm::vec3(frictionForce, 0, 0));
		}
	}

	else if (_velocity.x < 0)
	{
		if (_velocity.x > -0.01f)
		{
			_velocity.x = 0.0f;
		}
		else
		{
			AddForce(glm::vec3(-frictionForce, 0, 0));
		}
	}

	if (_velocity.z > 0)
	{
		if (_velocity.z < 0.01f)
		{
			_velocity.z = 0.0f;
		}
		else
		{
			AddForce(glm::vec3(0, 0, frictionForce));
		}
	}
	
	else if (_velocity.z < 0)
	{
		if (_velocity.z > -0.01f)
		{
			_velocity.z = 0.0f;
		}
		else
		{
			AddForce(glm::vec3(0, 0, -frictionForce));
		}
	}

}

void DynamicObject::UpdateModelMatrix()
{
	glm::mat4 model_rotation = glm::toMat4(_quatR);

	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = _modelMatrix * model_rotation;
	_modelMatrix = glm::scale(_modelMatrix, _scale);

	_invModelMatrix = glm::inverse(_modelMatrix);
}

void DynamicObject::SetScale(glm::vec3 scale)
{
	_scale = scale;
	_bRadius *= scale.x;
}