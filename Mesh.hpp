#ifndef MESH_HPP
# define MESH_HPP

# include <fstream>
# include <sstream>
# include <map>
# include <vector>

# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "vector.hpp"
# include "ProcMap.hpp"

# define THRESHOLD				(100 / 4.0f)
# define GRID					std::vector< std::vector<float> >

typedef struct					s_vec
{
	GLfloat						x;
	GLfloat						y;
	GLfloat						z;
}								t_vec;

typedef struct					s_vec2
{
	GLfloat						u;
	GLfloat						v;
}								t_vec2;

class							Mesh
{
	public:
								Mesh();
								Mesh(bool dynamic);
								Mesh(const std::string fileName);
								Mesh(const std::string fileName, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat *pos);

		void					bindVBO(GLuint program);
		void					bindVBOInstance(GLuint program);

		OpenGLMatrix*			getModelMatrix( void );
		void					setModelMatrix( OpenGLMatrix newMatrix );

		void					drawMesh( void ) const;
		void					drawMeshInstanced( void ) const;
		void					updateMesh(float t);

		void					sendPosition(unsigned int size, std::vector<t_vec> postion);
        GLuint          	    *getVBO( void );
        void            	    setNbParticles( int n );
		std::vector< t_vec >	getPosition() const;

	private:
		void					getFace(std::string face);
		void					loadOBJ(const std::string fileName);
		void					rearrange();

		void					createTerrain(bool top);
		void					createMap(GRID &map, int gridSize, float height, std::vector<t_vec> tops, float t);
		void					computeVert(GRID &map, int gridSize);
		void					computeNorm();
		void					computeIndices();
		float					calcUp(int i, int j, t_vec top);
		t_vec					getNorm(GLuint center, GLuint first, GLuint second);

	private:
		OpenGLMatrix						_modelMatrix;
		std::vector< t_vec >				_vertex;
		std::vector< t_vec >				_vertexN;
		std::vector< t_vec2 >				_vertexT;
		std::vector< t_vec >				_vertexOO;
		std::vector< t_vec >				_vertexNOO;
		std::vector< t_vec >				_vertexTOO;
		std::vector< GLuint >				_indices;
		std::vector< GLuint >				_indicesOO;
		std::map< std::string, GLuint >		_triplets;
		GLuint								_curInd;

		std::map<GLuint, t_vec>				_oneNorm;
		std::vector<GLuint>					_vecIds;
	
		GLuint								_vao;
		GLuint								_vbo[4];

		bool								_dynamic;
		int                   				_nbParticles;

		int									_gridSize;
		float								_maxHeight;
};

std::ostream&								operator<<(std::ostream& flux, Mesh const& m);

#endif
