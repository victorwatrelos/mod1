#include <stddef.h>
#include <unistd.h>

#include "OpenGLManager.hpp"
#include "FakeCamera.hpp"
#include "LookAtCamera.hpp"
#include "FreeCamera.hpp"

int					createScene(OpenGLScene *scene, std::string filename)
{
//	int				wID;
//	int				mID;
	int				pID;
//	int				oID;

//	wID = scene->addShaderProg("shader/water.vert", "shader/lightning.frag");
//	mID = scene->addShaderProg("shader/elevation.vert", "shader/lightning.frag");
	pID = scene->addShaderProg("shader/particles.vert", "shader/lightning.frag");
//	oID = scene->addShaderProg("shader/particles.vert", "shader/lightning.frag");
	if (pID == -1 /* || wID == -1 */)
		return (-1);
//	scene->addMesh(MESH_OBJ, filename, oID);
	scene->addParticleSystem(MESH_OBJ, filename, 10000, pID);
	return (1);
}

int					main(int ac, char **av)
{
	OpenGLManager	oGlMan(1920,1050, "Mod1");
	t_vecf			eye = {0.0f, 35.0f, -60.0f};
	CameraControl	*camera3 = new FreeCamera(eye, 0.4f, 0.0f);
	std::string		filename = "resources/softsphere.obj";
	OpenGLScene		*scene = new OpenGLScene;

	if (ac == 2)
		filename = av[1];
	if (createScene(scene, filename))
		oGlMan.run(camera3, scene);
	else
		std::cout << "Scene wasn't set, error occured." << std::endl;
	return 0;
}
