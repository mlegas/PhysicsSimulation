
#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <GLM/glm.hpp>
#include "glew.h"

/*! \brief
*  Material class encapsulates shaders and textures.
*  The class defines surface characteristics of geometry objects about how these object reflect light.
*  If your focus is on physics programming, you may not need to change this class.
*
*/

class Material
{
public:

	Material();
	~Material();

	/** Function for loading shaders from file
	* Returns false if there was an error. It will also print out messages to console.
	* @param vertFilename vertex shader file name
	* @param fragFilename fragment shader file name
	* @return the resullts
	*/
	bool LoadShaders( std::string vertFilename, std::string fragFilename ); 

	/** Function for setting the standard matrices needed by the shader
	* @param modelMatrix 4x4 Model matrix
	* @param invModelMatrix 4x4 Inverse model matrix
	* @param viewMatrix 4x4 Viewing matrix
	* @param projMatrix 4x4 Projection matrix
	* 
	*/
	void SetMatrices(glm::mat4 &modelMatrix, glm::mat4 &invModelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projMatrix); 
	
	/**Function for assigning emissive colour
	* @param input a 3D vector 
	*/
	void SetEmissiveColour( glm::vec3 input ) { _emissiveColour = input;} 
	/**Function for assigning diffuse colour
	* @param input a 3D vector
	*/
	void SetDiffuseColour( glm::vec3 input ) { _diffuseColour = input;}
	/**Function for assigning specular colour 
	* @param input a 3D vector
	*/
	void SetSpecularColour( glm::vec3 input ) { _specularColour = input;} 
	/**Function for assigning a light position in world space
	* @param input a 3D vector
	*/
	void SetLightPosition( glm::vec3 input ) { _lightPosition = input;} 


	/**Sets texture
	* This applies to ambient, diffuse and specular colours
	* If you want textures for anything else, you'll need to do that yourself.
	* @param filename 
	*/
	bool SetTexture( std::string filename ) {_texture1 = LoadTexture(filename); return _texture1>0;}

	/**Function for setting the material, applying the shaders 
	*/
	void Apply(); 

protected:

	bool CheckShaderCompiled( GLint shader ); /**< Utility function */ 
	
	int _shaderProgram; /**< The OpenGL shader program handle */ 

	/**
	* Locations of Uniforms in the vertex shader
	*/
	int _shaderModelMatLocation; /**< Model materix location */ 
	int _shaderInvModelMatLocation; /**< Inverse of the model matrix location */ 
	int _shaderViewMatLocation; /**< Viewing matrix location  */ 
	int _shaderProjMatLocation; /**< Projection matrix location */ 

	/**
	* Locations of Uniforms in the fragment shader
	*/
	int _shaderDiffuseColLocation; /**< Diffuse colour location */
	int _shaderEmissiveColLocation;/**< Emissive colour location  */
	int _shaderSpecularColLocation;/**< Specular colour location  */
	int _shaderWSLightPosLocation; /**< Light location */ 
	int _shaderTex1SamplerLocation; /**< Texture location */ 

	/**
	*Local store of material properties to be sent to the shader
	*/
	glm::vec3 _emissiveColour; /**<Emissive colour  */
	glm::vec3 _diffuseColour; /**< Diffuse colour */
	glm::vec3 _specularColour; /**< Specular colour */
	glm::vec3 _lightPosition;/**< Light position */ 

	unsigned int LoadTexture( std::string filename ); /**< Loads a .bmp from file  */ 
	
	unsigned int _texture1; /**< OpenGL handle for the texture */ 

};




#endif
