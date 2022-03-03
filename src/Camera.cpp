#include "Camera.h"

/*! \brief Brief description.
*  Camera class sets up parameters for camera. It controls camera's movement based on keyboard and mourse information.
*
*/
Camera::Camera()
{
	_cameraPos = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 initTarget = glm::vec3(0.0, 0.0f, -2.0f);

	_cameraAngleX = 3.14f;
	_cameraAngleY = 0.0f;

	speed = 1.1f;
	mouseSpeed = 0.005f;

	oldMousePosition = glm::vec2(0);

	_up = glm::vec3(0, 1, 0); 

	//Build  projection and viewing matrices
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f);
	_viewMatrix = glm::lookAt(_cameraPos, initTarget, _up);
}

Camera::~Camera() {}

void Camera::Update(Input* input)
{
	int Mid_X = 400;
	int Mid_Y = 300;
	// move the mouse back to the middle of the screen each frame
	SDL_WarpMouseInWindow(NULL, 400, 300);
	// get the current position of the cursor
	SDL_GetMouseState(&posx, &posy);

	float diff_x = Mid_X - oldMousePosition.x;
	float diff_y = Mid_Y - oldMousePosition.y;

	_cameraAngleX += mouseSpeed * (posx - oldMousePosition.x + diff_x);
	_cameraAngleY += mouseSpeed * (posy - oldMousePosition.y + diff_y);

	oldMousePosition = glm::vec2(posx, posy);

	glm::vec3 direction(cos(_cameraAngleY) * sin(_cameraAngleX), sin(_cameraAngleY), cos(_cameraAngleY) * cos(_cameraAngleX));
	glm::vec3 right = glm::vec3(sin(_cameraAngleX - 3.14f / 2.0f), 0, cos(_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	// Move camera in a direction
	if (input->cmd_w)
	{
		_translation = direction * speed;
		_cameraPos += _translation;

	}
	else if (input->cmd_s)
	{
		_translation = -(direction * speed);
		_cameraPos += _translation;
	}
	if (input->cmd_a)
	{
		_translation = -(right * speed);
		_cameraPos += _translation;

	}
	else if (input->cmd_d)
	{
		_translation = right * speed;
		_cameraPos += _translation;
	}

	// the matrices used to see the models are changed.
	//_projMatrix = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10000.0f);
	_viewMatrix = glm::lookAt(_cameraPos, _cameraPos + direction, up);

}