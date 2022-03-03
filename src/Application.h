#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <SDL.h>
#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations

#include <vector>
#include <iostream>

/*! \brief Brief description.
* 
* Application class defines the main sequences for the application (Init, Update, Exit, etc.)
* This is only an example class. You need to expand this class for your assignment.
* @Author: Wen Tang
*/

class Scene;
class Input;

class Application
{
private:
	/** SDL Variables and parameters
	*/
	SDL_Window* window; /*!<SDL window*/
	SDL_Renderer* renderer; /*!<SDL renderer*/
	SDL_Surface* surface; /*!<SDL window surface*/
	SDL_Event  events; /*!< The current event being parsed from SDL*/
	SDL_GLContext glcontext; /*!<gl drawing context*/

    /** Game engine variables and parameters
	*/
	bool running; /*!< Defines if the update loop is running or not*/
	
    /** Game and simulation timing variables and parameters
	*/
	unsigned int lastTime; /*!< The last frames time*/
	unsigned int currentTime; /*!< The current frames time */
	float deltaTime; /*!< The delta between the last frame and current frame times */

	/** Game and simulation content 
	*/
	Scene* myScene; /*!< The game scene*/
	Input* input; /*!< The keyboard and mouse input */
	
    /** STATIC MEMBERS: only one application instance running 
	*/
	static Application* instance;

public:
	/** Application class constructor
	*/
	Application();
	/** Application class distructor
	*/
	~Application();

	/** Initialisation of the application class
	*/
	bool Init();
	/** Initialises OpenGL
	*/
	bool InitGL();

	/** Updates the application's runtime
	*/
	bool Update();
	/** Exit sequence for the application
	*/
	bool Exit();
	/** Runs the application in the Init->Update->Exit order with error checking
	*/
	bool Run();

	/* STATIC METHODS*/
	static Application* Instance();
};

#endif // !_APPLICATION_H_

