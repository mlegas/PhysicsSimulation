#include "Application.h"
#include "glew.h"
#include "Scene.h"
#include <string>
#include <cstring>
#include <iostream>

/**
* Application class which defines the main sequences for the application (Init, Update and Exit, etc.)
* This is only an example class. There are many different ways of designing this class.
* For your physics computation, you may not need to modify this class.
* @file: Application.cpp
*/

// Define a fixed step length for stable physics simulations
static float STEP_LENGTH = 0.03;

Application::Application()
{
	/** Default parameters for SDL
	*/
	window = nullptr;
	renderer = nullptr;

	// Engine defaults
	running = false;

	//Timing defaults
	lastTime = 0;
	currentTime = 0;
	deltaTime = 0.0166666667f; // Default deltatime to 1/60 for first frame
	
}

Application::~Application()
{}


bool Application::Run()
{
	// Runs the application in the Init->Update->Exit order with error checking
	// Checks each run to ensure it returned true. If it didnt this will return false to main
	
	if (Init())
	{
		if (Update())
		{
			if (Exit())
			{
				// Returns true to main stating that everything ran okay and we can exit now.
				return true;
			}
		}
	}
	return false;
}

bool Application::Init()
{
	
	// Init SDL with video mode
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Failed to init SDL
		std::cerr << "Failed to init SDL!\n";
		return false;
	}

	// This is how we set the context profile through SDL to setup the OpenGL drawing contxt
	// Major version number (4):
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	// Minor version number (3):
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Then we say whether we want the core profile or the compatibility profile
	// Flag options are either: SDL_GL_CONTEXT_PROFILE_CORE   or   SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
	// We'll go for the core profile
	// This means we are using the latest version and cannot use the deprecated functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create the SDL window
	window = SDL_CreateWindow("Physics Simulation", 100, 100, 800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Check the window was made okay
	if (window == NULL)
	{
		std::cerr << "Failed to create SDL window!\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "Failed to create SDL Window!", NULL);
		return false;
	}

	// Create the SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	// This will allow us to actually use OpenGL to draw to the window
	glcontext = SDL_GL_CreateContext(window);

	// Call our initialisation function to set up GLEW and print out some GL info to console
	if (!InitGL())
	{
		return false;
	}

	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	// When you do this, don't forget to clear the depth buffer at the start of each frame - otherwise you just get an empty screen!
	glEnable(GL_DEPTH_TEST);

	// The scene contains all the objects etc
    myScene = new Scene();

	SDL_ShowCursor(true);
	input = new Input();

	// Get the last time to calculate deltatime for the first runthrough.
	/* No more code should be under this line in the init function unless its
	timing related or enabling the update loop */

	lastTime = SDL_GetTicks();
	running = true;
	
	return true;
}

bool Application::InitGL()
{
	// GLEW has a problem with loading core OpenGL
	// See here: https://www.opengl.org/wiki/OpenGL_Loading_Library
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	std::cerr << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cerr << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cerr << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cerr << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return true;
}

bool Application::Update()
{
	// Game loop
	while (running)
	{
		// Calculate deltatime
		currentTime = SDL_GetTicks();
		deltaTime = (float)(currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		Uint64 start = SDL_GetPerformanceCounter();

		// Update the scene
	    input->update();
		if (input->Quit)
			running = false;

		// Update scene and physics simulation
		// Run simulation: 
		// We target a real-time simulation, say 60 fps.
		// Therefore multiple updates to achieve our goal.

		double dt = STEP_LENGTH;
		int NumOfUpdates = int(deltaTime * 60);
		for (int i = 0; i < NumOfUpdates; i++)
		{
			myScene->Update(dt, input);
		}
		NumOfUpdates = 0;
		
		// Specify the colour to clear the framebuffer to
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		// This writes the above colour to the colour part of the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw the scene
		myScene->Draw();

		// This tells the renderer to actually show its contents to the screen
		// We'll get into this sort of thing at a later date - or just look up 'double buffering' if you're impatient :P
		SDL_GL_SwapWindow(window);

		//caps framerate
		if (deltaTime < (1.0f / 60.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 60.0f) - deltaTime)*1000.0f));
		}
		lastTime = currentTime;

		if (input->cmd_c)
		{
			delete myScene;
			myScene = new Scene();
		}
		
		Uint64 end = SDL_GetPerformanceCounter();

		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

		char title[9999];
		std::string text = "FPS: ";
		text += std::to_string(1.0f/elapsed);
		strcpy_s(title, text.c_str());

		SDL_SetWindowTitle(window, title);
	}

	return true;
}


bool Application::Exit()
{
	
	// Destroy everything, clean up!
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return true;
}

/** STATIC IMPLEMENTS 
* The static instance variable that stores the one instance of itself
*/
Application* Application::instance = nullptr;

/** The Instance getter method. Returns the static instance of itself (or makes one if it doesn't yet exist
*/
Application* Application::Instance()
{
	// Check if the instance is null
	if (instance == nullptr)
	{
		// Instance is null, we need to make one
		instance = new Application();
	}

	// Return the instance of the application
	return instance;
}