#include "OpenGLManager.hpp"
#include <unistd.h>

void					OpenGLManager::createProjectionMatrix(void)
{
	this->_clipInfo.aspect = this->_winInfo.width / this->_winInfo.height;
	this->_projectionMatrix.computeProjectionMatrix(this->_clipInfo.fov, this->_clipInfo.aspect, this->_clipInfo.zNear, this->_clipInfo.zFar);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	t_user_ptr				*ptr;

	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
	ptr->winInfo->width = w;
	ptr->winInfo->height = h;
    glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_user_ptr				*ptr;
	float					alpha = 0.5235f;

	(void)scancode;
	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else if (action == GLFW_PRESS)
	{
		if (mods == GLFW_MOD_SHIFT)
		{
			if (key == GLFW_KEY_LEFT)
				ptr->model->rotateZ(alpha);
			else if (key == GLFW_KEY_RIGHT)
				ptr->model->rotateZ(-alpha);
		}
		else
		{
			if (key == GLFW_KEY_UP)
				ptr->model->rotateX(alpha);
			else if (key == GLFW_KEY_DOWN)
				ptr->model->rotateX(-alpha);
			else if (key == GLFW_KEY_LEFT)
				ptr->model->rotateY(alpha);
			else if (key == GLFW_KEY_RIGHT)
				ptr->model->rotateY(-alpha);
		}
	}
	ptr->camera->controlKey(key, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	t_user_ptr				*ptr;
	
	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
	ptr->camera->controlMouse(xPos, yPos);
}

void error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}

void				OpenGLManager::initOpenGl( void )
{
    int     width, height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(0);

	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
//	glfwWindowHint(GLFW_DEPTH_BITS, 24);
  	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->_window = glfwCreateWindow(this->_winInfo.width, this->_winInfo.height, this->_winInfo.winName.c_str(), NULL, NULL);
    if (! this->_window )
    {
        glfwTerminate();
        exit(0);
    }
    
    glfwSetFramebufferSizeCallback(this->_window, framebuffer_size_callback);
    glfwSetKeyCallback(this->_window, key_callback);
    glfwSetCursorPosCallback(this->_window, cursor_position_callback);
	glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwMakeContextCurrent(this->_window);
    glfwSwapInterval(1);
  //  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwGetFramebufferSize(this->_window, &(width), &(height));
    glViewport(0, 0, width, height);
    //chdir("../");//Glfw change the working dir to /ressources, so this is to go back to root.
}

OpenGLManager::OpenGLManager()
{
	_winInfo.width = 512;
	_winInfo.height = 512;
	_winInfo.winName = "Untitled";
	_clipInfo.fov = 45;
	_clipInfo.aspect = 1;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 10000;
	initOpenGl();
}

OpenGLManager::OpenGLManager( GLfloat width, GLfloat height, std::string winName ) 
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = 45;
	_clipInfo.aspect = width / height;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 1000;
	initOpenGl();
}

OpenGLManager::OpenGLManager( GLfloat width, GLfloat height, std::string winName,
				 			   GLfloat fov, GLfloat aspect, 
							   GLfloat zNear, GLfloat zFar )
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = fov;
	_clipInfo.aspect = aspect;
	_clipInfo.zNear = zNear;
	_clipInfo.zFar = zFar;
	initOpenGl();
}

OpenGLManager::~OpenGLManager()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int				OpenGLManager::shouldClose()
{
	return (glfwWindowShouldClose(_window));
}

void				OpenGLManager::swap()
{
	glfwSwapBuffers(_window);
}

void				OpenGLManager::setUserPtr(t_user_ptr *s)
{
	glfwSetWindowUserPointer(_window, static_cast<void *>(s));
}

void				OpenGLManager::run(CameraControl *cam, OpenGLScene *scene)
{
	float			t = 0.f;
	OpenGLMatrix	modelMat;
	t_user_ptr		*userPtr = new t_user_ptr;

	this->createProjectionMatrix();
	userPtr->winInfo = &this->_winInfo;
	userPtr->model = scene->getModelMatrix();
	userPtr->camera = cam;
	this->setUserPtr(userPtr);

    while (!this->shouldClose())
    {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->drawScene(cam->getViewMatrix(), this->_projectionMatrix, t);

		t += .2f;
		if (t > 1024.f)
			t = 0.f;

		this->swap();
		glfwPollEvents();
	}
}
