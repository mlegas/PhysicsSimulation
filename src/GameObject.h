#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object. 
*
*/

class GameObject
{
public:
	/** GameObject constructor
	* 
	*/
	GameObject();
	/** GameObject distructor
	*/
	~GameObject();

	/** Function for setting mesh geometry for the game object 
	* @param *input  a pointer to a mesh object
	*/
	void SetMesh(Mesh *input) {_mesh = input;}
	/** Function for setting material for the game object
	* @param *input  a pointer to a material object
	*/
	void SetMaterial(Material *input) {_material = input;}
	/** Function for setting position for the game object
	* @param float posX x position
	* @param float posY y position
	* @param float posZ z position
	*/
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}
	/** Function for setting position for the game object
	* @param glm::vec3 value  a position 3D vector
	*/
	void SetPosition( glm::vec3 value) {_position = value;}
	/** Function for setting rotation for the game object
	* @param float rotX x rotation
	* @param float rotY y rotation
	* @param float rotZ z rotation
	*/
	void SetRotation( float rotX, float rotY, float rotZ ) {_rotation.x = rotX; _rotation.y = rotY; _rotation.z = rotZ;}
	/** Function for setting scale for the game object
	* @param float sX x scale
	* @param float sY y scale
	* @param float sZ z scale
	*/
	void SetScale(float sX, float sY, float sZ) { _scale.x = sX; _scale.y = sY; _scale.z = sZ; }
	
	/** Function for getting position of the game object
	* @return The result
	*/
	glm::vec3 GetPosition() {return _position;}
	
	/** A virtual function for updating the simulation result at each time frame
	*   You need to expand this function 
	* @param float deltaTs the time intervel in second for the simulation frame
	*/
	virtual void Update( float deltaTs );
	/** A virtual function for drawing the simulation result
	*  The function takes viewing matrix and projection matrix 
	* @param glm::mat4 &viewMatrix a 4x4 matrix
	* @param glm::mat4 &projMatrix a 4x4 matrix
	*/
	virtual void Draw(glm::mat4 &viewMatrix, glm::mat4 &projMatrix);

protected:

	/** The model geometry
	*/
	Mesh *_mesh;
	/** The material contains the shader
	*/
	Material *_material;

	/** Matrix for the position of the game object
	*/ 
	glm::mat4 _modelMatrix;
	/** Matrix for the orientation of the game object
	*/
	glm::mat4 _invModelMatrix;
	
	/** Position of the model
	* The model matrix must be built from the position of the model geometry
	*/
	glm::vec3 _position;
	
	/** Orientation of the model
	* The model matrix must be built from the orientation of the model geometry
	*/
	glm::vec3 _rotation;
	/** Scale of the model
	* The model matrix must be built from the scale of the model geometry
	*/
	glm::vec3 _scale;

};



#endif
