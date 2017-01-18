#include "OpenGLShader.hpp"
#include <iostream>

char		*OpenGLShader::filetobuf(const char *file)
{
    FILE *fptr;
    long length;
    char *buf;
    
    fptr = fopen(file, "r");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length+1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

void			OpenGLShader::deleteShader()
{
	for (auto i = _shaders.begin() ; i < _shaders.end() ; i++)
	{
		glDeleteShader(*i);
	}
}

int				OpenGLShader::addShader(GLenum type, std::string filename)
{
    GLuint shader;
    GLint shader_ok;
    GLsizei log_length;
    char info_log[8192];
	const char	*source;
    
	std::cout << filename << std::endl << std::flush;
	if ((source = filetobuf(filename.c_str())) == NULL)
	{
		return 0;
	}
    shader = glCreateShader(type);
    if (shader != 0)
    {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
        if (shader_ok != GL_TRUE)
        {
            fprintf(stderr, "ERROR: Failed to compile %s shader\n", (type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex" );
            glGetShaderInfoLog(shader, 8192, &log_length,info_log);
            fprintf(stderr, "ERROR: \n%s\n\n", info_log);
            glDeleteShader(shader);
            shader = 0;
			return (0);
        }
    }
	_shaders.push_back(shader);
    return (1);
}

int				OpenGLShader::createProgram()
{
    GLuint program = 0u;
    GLint program_ok;
    GLsizei log_length;
    char info_log[8192];
    
    /* make the program that connect the two shader and link it */
    program = glCreateProgram();
    if (program != 0u)
    {
		/* attach both shader and link */
		for (auto i = _shaders.begin() ; i < _shaders.end() ; i++)
		{
			glAttachShader(program, *i);
		}
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
		if (program_ok != GL_TRUE)
		{
			fprintf(stderr, "ERROR, failed to link shader program\n");
			glGetProgramInfoLog(program, 8192, &log_length, info_log);
			fprintf(stderr, "ERROR: \n%s\n\n", info_log);
			glDeleteProgram(program);
			deleteShader();
			program = 0u;
			return (0);
		}
    }
	_shaderProgram = program;
    return (1);
}

GLuint			OpenGLShader::getProgram()
{
	return (_shaderProgram);
}


