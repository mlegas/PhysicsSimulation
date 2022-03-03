#include "Scene.h"
#include "DynamicObject.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

/*! \brief Brief description.
*  Scene class is a container for loading all the game objects in your simulation or your game.
*
*/
Scene::Scene()
{
	std::ifstream textFile;
	std::string text;
	float gravity;
	float friction;
	float scale;
	float mass;
	float bounciness;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotation;
	int spheres;
	int planes;

	// Set up your scene here......
	// Set a camera
	_camera = new Camera();

	// Position of the light, in world-space
	_lightPosition = glm::vec3(10, 10, 0);

	// Create the material for the game object- level
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour(glm::vec3(0.8, 0.8, 0.8));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("assets/textures/diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material

	// The mesh is the geometry for the object
	Mesh *groundMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	groundMesh->LoadOBJ("assets/models/woodfloor.obj");
	// Tell the game object to use this mesh

	// Create the material for the game object- level
	Material *objectMaterial = new Material();
	// Shaders are now in files
	objectMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	objectMaterial->SetDiffuseColour(glm::vec3(0.8, 0.1, 0.1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	objectMaterial->SetTexture("assets/textures/default.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	objectMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material

	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	// Tell the game object to use this mesh

	srand(time(NULL));

	try
	{
		textFile.open("simulation.txt");
	}
	catch (const std::exception&)
	{
		std::cout << "File not found!" << std::endl;
	}

	while (!textFile.eof())
	{
		textFile >> text;
		if (text == "Gravity")
		{
			textFile >> gravity;
		}
		else if (text == "Friction")
		{
			textFile >> friction;
		}
		else if (text == "Mass")
		{
			textFile >> mass;
		}
		else if (text == "NumberOfSpheres")
		{
			textFile >> spheres;

			for (int i = 0; i < spheres; i++)
			{
				std::shared_ptr<DynamicObject> sphere = std::make_shared<DynamicObject>();

				sphere->SetMaterial(objectMaterial);
				sphere->SetMesh(modelMesh);

				sphere->SetMass(mass);
				sphere->SetGravity(gravity);
				sphere->SetFriction(friction);

				for (int i = 0; i < 3; i++)
				{
					textFile >> text;
					if (text == "Position")
					{
						textFile >> position.x >> position.y >> position.z;

						sphere->SetPosition(position);
					}
					else if (text == "Velocity")
					{
						textFile >> velocity.x >> velocity.y >> velocity.z;

						sphere->SetVelocity(velocity);
					}
					else if (text == "Scale")
					{
						textFile >> scale;

						sphere->SetScale(glm::vec3(scale, scale, scale));
					}
				}
				_spheres.push_back(sphere);
			}
		}
		else if (text == "NumberOfPlanes")
		{
			textFile >> planes;

			for (int i = 0; i < planes; i++)
			{
				std::shared_ptr<Plane> plane = std::make_shared<Plane>();

				plane->SetMaterial(modelMaterial);
				plane->SetMesh(groundMesh);

				for (int i = 0; i < 4; i++)
				{
					textFile >> text;
					if (text == "Position")
					{
						textFile >> position.x >> position.y >> position.z;

						plane->SetPosition(position);
					}
					else if (text == "Rotation")
					{
						textFile >> rotation.x >> rotation.y >> rotation.z;

						plane->SetRotation(rotation.x, rotation.y, rotation.z);
					}
					else if (text == "Scale")
					{
						textFile >> scale;

						plane->SetScale(scale, scale, scale);
					}
					else if (text == "Bounciness")
					{
						textFile >> bounciness;

						plane->SetBounciness(bounciness);
					}
				}

				_planes.push_back(plane);
			}
		}
	}
}

Scene::~Scene()
{
	// You should neatly clean everything up here
	delete _camera;
}

void Scene::Update(float deltaTs, Input* input)
{
	// Update the game object (this is currently hard-coded motion)
	if (input->cmd_x)
	{
		if (!_spheres.empty())
		{
			for (int i = 0; i < _spheres.size(); i++)
			{
				_spheres.at(i)->StartSimulation(true);
			}
		}
	}

	if (!_spheres.empty())
	{
		for (int i = 0; i < _spheres.size(); i++)
		{
			_spheres.at(i)->Update(deltaTs, _spheres, _planes);
		}
	}

	if (!_planes.empty())
	{
		for (int i = 0; i < _planes.size(); i++)
		{
			_planes.at(i)->Update(deltaTs);
		}
	}
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	if (!_spheres.empty())
	{
		for (int i = 0; i < _spheres.size(); i++)
		{
			_spheres.at(i)->Draw(_viewMatrix, _projMatrix);
		}
	}

	if (!_planes.empty())
	{
		for (int i = 0; i < _planes.size(); i++)
		{
			_planes.at(i)->Draw(_viewMatrix, _projMatrix);
		}
	}

}