#include "Rigidbody.h"
#include "Utility.h"
#include <glm/ext.hpp>

Rigidbody::Rigidbody()
{
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
	// Set default value
	_bRadius = 0.15f;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_mass = 2.0f;
	_start = false;

	_velocity = glm::vec3(0.0f, 1.0f, 1.0f);

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

Rigidbody::~Rigidbody()
{

}

void Rigidbody::Update(float deltaTs)
{
	if (_start == true)
	{
		ClearForces();
		ClearTorques();

		glm::vec3 f = ComputeForces();

		AddForce(f);

		CollisionResponses(deltaTs);

		//	if (_position.y < 0.3f)
		//	{
		//		_position.y = 0.3f;
		//		glm::vec3 bounceForce = glm::vec3(0.0f, 150.0f, 0.0f);
		//		AddForce(bounceForce);
		//	}

			//RungeKutta2(deltaTs);
		RungeKutta4(deltaTs);
		//Euler(deltaTs);

	}
	UpdateModelMatrix();
}

void Rigidbody::RungeKutta2(float deltaTs)
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
	acceleration = force * (1 / _mass);
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

	glm::mat3 omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += omega_star * _R * deltaTs;

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);

	_velocity += k1;
	_position += _velocity * deltaTs;
}

void Rigidbody::RungeKutta4(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 torque;
	glm::vec3 k0;
	glm::vec3 k1;
	glm::vec3 k2;
	glm::vec3 k3;
	glm::mat3 omega_star;
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

	omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += omega_star * _R * deltaTs;

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);

	_velocity += (k0 + 2.0f * k1 + 2.0f * k2 + k3) / 6.0f;
	_position += _velocity * deltaTs;
}

glm::vec3 Rigidbody::ComputeForces()
{
	return _freefall * _mass;
}

void Rigidbody::ComputeInverseInertiaTensor()
{
	_inertia_tensor_inverse = _R * _body_inertia_tensor_inverse * glm::transpose(_R);
}

void Rigidbody::Euler(float deltaTs)
{
	_velocity += (_force / _mass) * deltaTs;
	_position += _velocity * deltaTs;

	_angular_momentum += _torque * deltaTs;

	ComputeInverseInertiaTensor();

	_angular_velocity = _inertia_tensor_inverse * _angular_momentum;

	glm::mat3 omega_star = glm::mat3(0.0f, -_angular_velocity.z, _angular_velocity.y,
		_angular_velocity.z, 0.0f, -_angular_velocity.x,
		-_angular_velocity.y, _angular_velocity.x, 0.0f);

	_R += omega_star * _R * deltaTs;

	_quatR = glm::toQuat(_R);
	_quatR = glm::normalize(_quatR);

}

void Rigidbody::CollisionResponses(float deltaTs)
{
	glm::vec3 position = GetPosition();

	glm::vec3 n(0.0f, 1.0f, 0.0f);
	glm::vec3 q(0.0f, 0.0f, 0.0f);

	float bRadius = GetBoundingRadius();

	glm::vec3 velocity = _velocity;
	glm::vec3 contactPosition;
	glm::vec3 c1 = position + velocity * deltaTs;
	glm::vec3 c0 = position;

	bool collision = PFG::MovingSphereToPlaneCollision2(n, c0, c1, q, bRadius, contactPosition);

	if (collision)
	{
		float impulse = (-(1 + 0.9)  * glm::dot(_velocity, n)) / (1 / _mass);
		glm::vec3 impulseForce = impulse * n / deltaTs;
		AddForce(impulseForce + glm::vec3(0.0f, 9.8f*_mass, 0.0f));

		glm::vec3 temp = contactPosition - _bRadius;

		glm::vec3 force2 = impulse * glm::vec3(1.0f, 1.0f, 1.0f)*0.02f;
		glm::vec3 torque = glm::cross(temp, force2);

		_angular_momentum *= impulse / 50;

		AddTorque(torque);

	}
}

void Rigidbody::UpdateModelMatrix()
{
	glm::mat4 model_rotation = glm::toMat4(_quatR);

	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = _modelMatrix * model_rotation;
	_modelMatrix = glm::scale(_modelMatrix, _scale);

	_invModelMatrix = glm::inverse(_modelMatrix);
}