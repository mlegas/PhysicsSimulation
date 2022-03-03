#ifndef _DynamicObject_H_
#define _DynamicObject_H_

#include "GameObject.h"
#include "Utility.h"
#include <glm/gtx/quaternion.hpp>

/*! \brief Brief description.
*  This physics dynamic object class is derived from the GameObject class, as a one type/class of game objects
*  It sets up parameters for physics computation and calls certain physics algorithms.
*  It returns the position and orientation of the object for the visualisation engine to display.
*  It is important to not include any graphics drawings in this class. This is the principle of the separation
*  of physics computation from graphics
*
*/

class DynamicObject : public GameObject
{
public:

	/** DynamicObject constructor
	*/
	DynamicObject();
	/** DynamicObject distructor
	*/
	~DynamicObject();

	/** Update function to override the base class function
	*   for physics computation
	*   This function is typically organised that it calls each physics computation procedures/algorithms
	*   in a sequential order, so that each computational algorithm provides required information for the
	*   next procedure.
	*   @param float deltaTs simulation time step length
	*/
	virtual void Update(float deltaTs);

	/** Add force that acts on the object to the total force for physics computation
	*
	*   @param const glm::vec3 force
	*/
	void AddForce(const glm::vec3 force) { _force += force; }
	void ClearForces() { _force = glm::vec3(0.0f, 0.0f, 0.0f); }
	/** Numerical integration function to compute the current velocity and the current position
	* based on the velocity and the position of the previous time step
	*   @param float deltaTs simulation time step length
	*/

	void AddTorque(const glm::vec3 torque) { _torque += torque; }
	void ClearTorques() { _torque = glm::vec3(0.0f, 0.0f, 0.0f); }
	void ComputeInverseInertiaTensor();

	void Euler(float deltaTs);

	void RungeKutta2(float deltaTs);

	void RungeKutta4(float deltaTs);

	void CollisionResponses(float deltaTs);

	glm::vec3 ComputeForces();

	/** Set force for the object
	* @param glm::vec3 force a 3D vector for the force acting on the object
	*/
	void SetForce(const glm::vec3 force) { _force = force; }
	/** Set mass for the object
	* @param float mass a 3D vector for the mass of the object
	*/
	void SetMass(float mass) { _mass = mass; }
	/** Set a sphere bounding volume for the object
	* @param float r  the radius of the bounding sphere of the object
	*/
	void SetBoundingRadius(float r) { _bRadius = r; }

	/** Set postion for the object
	* @param glm::vec3 pos a 3D vector for the position of the object
	*/
	void SetPosition(const glm::vec3 pos) { _position = pos; }
	/** Set velocity for the object
	* @param glm::vec3 vel a 3D vector for the velocity of the object
	*/
	void SetVelocity(const glm::vec3 vel) { _velocity = vel; }
	/** Set scale for the object
	* @param glm::vec3 vel a 3D vector for the scale of the object
	*/
	void SetScale(const glm::vec3 scale) { _scale = scale; }

	/** Get the force acting on the object
	* @return a 3D vector
	*/
	const glm::vec3 GetForce() const { return _force; }
	/** Get the mass of the object
	* @return the result
	*/
	const float GetMass() const { return _mass; }
	/** Get the radius of the bounding sphere of the object
	* @return the result
	*/
	const float GetBoundingRadius() const { return _bRadius; }
	/** Get the position of the object
	* @return a 3D vector
	*/
	const glm::vec3 GetPosition() const { return _position; }
	/** Get the orientation of the object
	* @return a 4x4 matrix
	*/
	const glm::mat4 GetOrientation() const { return _orientation; }

	/** A boolean variable to control the start of the simulation This matrix is the camera's lens
	*/
	void StartSimulation(bool start) { _start = start; }

private:

	/**Update the model matrix with the current position, orientation and scale
	*
	*/
	void UpdateModelMatrix();

	/** Set up physics parameters for computation
	*  Specific parameters are determined by the physics simulation
	*/
	/** The total force on the object
	*/
	glm::vec3 _force;
	/** Position of the object
	*/
	glm::vec3 _position;
	/** Velocity of the object
	*/
	glm::vec3 _velocity;
	/** The mass of the object
	*/

	glm::mat3 omega_star;
	
	glm::vec3 _torque;
	glm::vec3 _angular_velocity;
	glm::vec3 _angular_momentum;
	glm::mat3 _inertia_tensor_inverse;
	glm::mat3 _body_inertia_tensor_inverse;
	glm::mat3 _R;
	glm::quat _quatR;

	float _mass;
	/** Scale of the object
	*/
	/** The radius of a bounding sphere of the object
	*/
	float _bRadius;
	/** Scale of the object
	*/
	glm::vec3 _scale;
	/** Orientation of the object
	*/
	glm::mat4 _orientation;

	/** A boolean variable to control the start of the simulation This matrix is the camera's lens
	*/
	bool _start;

	glm::vec3 _freefall = glm::vec3(0, -9.81f, 0);
};

#endif //!_DynamicObject_H_
