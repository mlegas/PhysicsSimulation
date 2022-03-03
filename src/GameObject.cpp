#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "GameObject.h"

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object.
*
*/
GameObject::GameObject()
{
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
	// Set default value
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

GameObject::~GameObject()
{
	// Do any clean up here
}

void GameObject::Update( float deltaTs )
{
	// Put any update code here
	// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
	//_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0,1,0) );
	//_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0,1,0) );
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1, 0, 0));
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0, 0, 1));
	_modelMatrix = glm::scale(_modelMatrix, _scale);
	_invModelMatrix = glm::inverse(_modelMatrix);
}

void GameObject::Draw(glm::mat4 &viewMatrix, glm::mat4 &projMatrix)
{
	if( _mesh != NULL )
	{
		if( _material != NULL )
		{
			// Give all the matrices to the material

			// This makes sure they are sent to the shader
			_material->SetMatrices(_modelMatrix, _invModelMatrix, viewMatrix, projMatrix);
			// This activates the shader
			_material->Apply();
		}

		// Sends the mesh data down the pipeline
		_mesh->Draw();

	}
}
