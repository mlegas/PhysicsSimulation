#ifndef __MESH__
#define __MESH__

#include <glm/glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <string>

/*! \brief
*  Mesh class is for loading a triangulated mesh from OBJ file and keeping a reference for it.
*  If your focus is on physics programming, If your focus is on physics programming, you don’t need to change this class.
*  
*/

class Mesh
{
public:

	/** The Mesh class constructor */
	Mesh();  
	/** The Mesh class distructor */
	~Mesh();
	
	
	/** Process an OBJ file
	*  The OBJ file must be triangulated
    */
	void LoadOBJ( std::string filename );

	/**Draws the mesh -
	*  The mesh must have shaders applied for this to display!
	*/
	void Draw();

protected:
	

	/**OpenGL Vertex Array Object
	*/
	GLuint _VAO;

	/**Number of vertices in the mesh
	*/
	unsigned int _numVertices;

};


#endif
