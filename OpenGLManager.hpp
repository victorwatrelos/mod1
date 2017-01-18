#ifndef OPENGLMANAGER_HPP
# define OPENGLMANAGER_HPP

# include <math.h>
# include <iostream>
# include <vector>
# include <string>

# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "OpenGLScene.hpp"
# include "CameraControl.hpp"

#define FROMFPS(X)				( 1.0f / X )
#define TOFPS(X)				( 1.0f / X )

#define AN_INT					FROMFPS(3.0f)

typedef struct					s_window_info
{
	GLfloat						width;
	GLfloat						height;
	std::string					winName;
}								t_window_info;

typedef struct					s_clipping_info
{
	GLfloat						fov;
	GLfloat						aspect;
	GLfloat						zNear;
	GLfloat						zFar;
}								t_clipping_info;

typedef struct					s_user_ptr
{
	t_window_info				*winInfo;
	OpenGLMatrix				*model;
	CameraControl				*camera;
}								t_user_ptr;

class							OpenGLManager
{
	public:
								OpenGLManager();
								OpenGLManager( GLfloat width, GLfloat height, std::string winName );
								OpenGLManager( GLfloat width, GLfloat height, std::string winName,
											   GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar );
								~OpenGLManager();
		int						shouldClose( void );
		void					swap( void );
		void					setUserPtr( t_user_ptr *s );
		void					createProjectionMatrix( void );
		void					run(CameraControl *cam, OpenGLScene *scene);

	private:
		void					initOpenGl( void );

	private:
    	GLFWwindow				*_window;
		OpenGLMatrix			_projectionMatrix;
		t_window_info			_winInfo;
		t_clipping_info			_clipInfo;

};

#endif //OPENGLMANAGER_HPP
