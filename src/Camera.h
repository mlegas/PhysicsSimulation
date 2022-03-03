#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include "Input.h"

/*! \brief Brief description.
*  Camera class sets up parameters for camera. It controls camera's movement based on keyboard and mourse information.
*
*/
class Camera
{
public:
	/** Camera constructor
	*/
	Camera();
	/** Camera distructor
	*/
	~Camera();
	/** Function for getting the viewing matrix
	* @return a 4x4 matrix
	*/
	inline glm::mat4 GetView() { return _viewMatrix; };
	/** Function for getting the projection matrix
	* @return a 4x4 matrix
	*/
	inline glm::mat4 GetProj() { return _projMatrix; };
	/** Function for getting the camera position
	* @param glm::vec3 pos camera position 
	*/
	inline void SetPos(glm::vec3 pos) { _cameraPos = pos; }

	/** Function for updatting the camera 
	* @param input keyboard and mourse input inofrmation
	*/
	void Update(Input* input);

private:
	/** Viewing matrix for the camera
	*/
	glm::mat4 _viewMatrix;
	/** Projection matrix for the camera
	*/
	glm::mat4 _projMatrix;	
	/** Position of the camera
	*/
	glm::vec3 _cameraPos;
	/** Orientation of the camera
	*/
	glm::vec3 _cameraRot;
	/** Point that camera looks at
	*/
	glm::vec3 _lookAt;
	/** Camera's up vector 
	*/
	glm::vec3 _up;

	/** Camera's transaltion vector
	*/
	glm::vec3 _translation;
	/** Camera's rotation angle in a radian about x axis
	*/
	float _cameraAngleX;
	/** Camera's rotation angle in a radian about y axis
	*/
	float _cameraAngleY;
	/** Camera's speed of movement 
	*/
	float speed;
	/** The speed of the mouse movement
	*/
	float mouseSpeed;
	/** The state of the mouse position
	*/
	int posx, posy;
	/** The state of the mouse's previous position
	*/
	glm::vec2 oldMousePosition;

};

#endif // !_CAMERA_H_