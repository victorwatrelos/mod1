#include "OpenGLScene.hpp"

OpenGLScene::OpenGLScene() : _progID(0), _isOpenCL(false)
{
}

OpenGLScene::~OpenGLScene()
{
	if (this->_isOpenCL)
		this->_openCL->release();
}

int					OpenGLScene::addShaderProg(std::string VSFile, std::string FSFile)
{
	OpenGLShader	*shader = new OpenGLShader;

	shader->addShader(GL_VERTEX_SHADER, VSFile);
	shader->addShader(GL_FRAGMENT_SHADER , FSFile);
	
	if (! shader->createProgram())
		return (-1);

	this->_progs.push_back(shader);
	this->_progID++;
	std::cout << "Program created: " << this->_progs[this->_progID - 1]->getProgram() << std::endl;
	return (this->_progID - 1);
}

int					OpenGLScene::addMesh(int type, int progID)
{
	Mesh			*mesh;
	switch (type)
	{
		case MESH_WATER:
			mesh = new Mesh(true);
			break;
		case MESH_MOUNT:
			mesh = new Mesh;
			break;
		default:
			return (-1);
	}
	mesh->bindVBO(this->_progs[progID]->getProgram());
	this->_meshes[progID].push_back(mesh);
	return (progID);
}

int					OpenGLScene::addMesh(int type, std::string filename, int progID)
{
	Mesh			*mesh;
	if (type == MESH_OBJ)
	{
		mesh = new Mesh(filename);
		mesh->bindVBO(this->_progs[progID]->getProgram());
		this->_meshes[progID].push_back(mesh);
		return (progID);
	}
	return (-1);
}

int					OpenGLScene::addLight(int type)
{
	(void)type;
    return 0;
}

int					OpenGLScene::addParticleSystem(int type, std::string filename, int nb, int progID)
{
	Mesh			*mesh;
	this->_isOpenCL = true;

	mesh = new Mesh(filename);
	mesh->setNbParticles(nb);
	mesh->bindVBOInstance(this->_progs[progID]->getProgram());
	this->_meshes[progID].push_back(mesh);

	this->_openCL = new OpenCL(nb, 2.0f, 4000, 30, 30, 30);
	this->_openCL->initOpenCL(mesh->getVBO()[3]);
	(void)type;
	return (1);
}

int					OpenGLScene::drawScene(OpenGLMatrix view, OpenGLMatrix project, float t)
{
	GLuint			progID;

	for (const auto& itMap : this->_meshes)
	{
		progID = this->_progs[itMap.first]->getProgram();
		glUseProgram(progID);
		for (const auto& itVector : itMap.second)
		{
			this->addMatricesToProgram(progID, *(itVector->getModelMatrix()), view, project, 50);
			itVector->updateMesh(t);
			itVector->drawMeshInstanced();
		}
		glUseProgram(0);
	}
	if (this->_isOpenCL)
	{
		glFinish();
		this->_openCL->executeKernel();
	}
	return (1);
}

void					OpenGLScene::addMatricesToProgram(GLuint progID, OpenGLMatrix model, OpenGLMatrix view, OpenGLMatrix project, float h)
{
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;
	GLint			uloc_H;

    uloc_M = glGetUniformLocation(progID, "M");
    uloc_V = glGetUniformLocation(progID, "V");
    uloc_P = glGetUniformLocation(progID, "P");
    uloc_H = glGetUniformLocation(progID, "H");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, project.getMatrixArray());
    glUniform1f(uloc_H, h);
}

OpenGLMatrix*		OpenGLScene::getModelMatrix()
{
	OpenGLMatrix	*modelMat;

	for (const auto& itMap : this->_meshes)
		modelMat = itMap.second[0]->getModelMatrix();
	return modelMat;
}
