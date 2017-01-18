#ifndef OPENGLSHADER_HPP
# define OPENGLSHADER_HPP

# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>

# include "OpenGLInc.hpp"

class					OpenGLShader
{
	
	public:
		int						addShader( GLenum type, std::string filename );
		int						createProgram( void );
		GLuint					getProgram( void );

		char					*filetobuf( const char *file );
		void					deleteShader( void );
	private:
    	GLuint					_shaderProgram;
		std::vector<GLuint>		_shaders;
};

#endif
