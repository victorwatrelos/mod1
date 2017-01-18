#ifndef OPENGLSCENE_HPP
# define OPENGLSCENE_HPP

# include <map>
# include <vector>
# include "OpenCL.hpp"
# include "OpenGLShader.hpp"
# include "Mesh.hpp"

# define MESH_OBJ			0
# define MESH_WATER			1
# define MESH_MOUNT			2
# define LIGHT				3

class						OpenGLScene
{
	public:
							OpenGLScene();
							~OpenGLScene();
		int					addShaderProg(std::string VSFile, std::string FSFile);
		int					addMesh(int type, int progID);
		int					addMesh(int type, std::string filename, int progID);
		int					addLight(int type);
		int					addParticleSystem(int type, int nb, int progID);
		int					addParticleSystem(int type, std::string filename, int nb, int progID);

		int					drawScene(OpenGLMatrix view, OpenGLMatrix project, float t);
		void				addMatricesToProgram(GLuint progID, OpenGLMatrix model, OpenGLMatrix view, OpenGLMatrix project, float h);
OpenGLMatrix*		getModelMatrix();

		std::map< int, std::vector< Mesh* > >	_meshes;
	private:
		int										_progID;
		std::vector< OpenGLShader* >			_progs;
		OpenCL									*_openCL;
		bool									_isOpenCL;
};


#endif
