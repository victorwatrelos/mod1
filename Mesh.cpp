#include "Mesh.hpp"

Mesh::Mesh() : _dynamic(false)
{
	this->_gridSize = 100;
	this->_maxHeight = 50.f;
	createTerrain(true);
}

Mesh::Mesh(bool dynamic) : _dynamic(dynamic)
{
	this->_gridSize = 100;
	this->_maxHeight = 50.f;
	createTerrain(false);
}

Mesh::Mesh(const std::string fileName)
{
	this->_curInd = 0;
	this->_dynamic = false;
	loadOBJ(fileName);
}

Mesh::Mesh(const std::string fileName, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat pos[3])
{
	this->_curInd = 0;
	this->_dynamic = false;
	loadOBJ(fileName);
	this->_modelMatrix.scale( scale );
	this->_modelMatrix.rotateX( rotX );
	this->_modelMatrix.rotateY( rotY );
	this->_modelMatrix.rotateZ( rotZ );
	this->_modelMatrix.translate( pos[0], pos[1], pos[2] );
}

void							Mesh::bindVBO(GLuint program)
{
	GLuint						attrloc;
	GLenum						usage = GL_STATIC_DRAW;

	if (this->_dynamic)
		usage = GL_DYNAMIC_DRAW;

	std::cout << "binding vbo to program: " << program << std::endl; 
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(3, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexOO.size(), &_vertexOO[0], usage);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexNOO.size(), &_vertexNOO[0], usage);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indicesOO.size(), &_indicesOO[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void							Mesh::bindVBOInstance(GLuint program)
{
	GLuint						attrloc;

	std::cout << "binding vbo to program: " << program << std::endl; 
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(4, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexOO.size(), &_vertexOO[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexNOO.size(), &_vertexNOO[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indicesOO.size(), &_indicesOO[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * this->_nbParticles, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(program, "instancePosition");
	glEnableVertexAttribArray(attrloc);
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(attrloc, 1);

	glBindVertexArray(0);
}

void							Mesh::drawMesh() const
{
	glBindVertexArray(this->_vao);
	glDrawElements(GL_TRIANGLES, this->_indicesOO.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void							Mesh::drawMeshInstanced() const
{
	glBindVertexArray(this->_vao);
	glDrawElementsInstanced(GL_TRIANGLES, this->_indicesOO.size(), GL_UNSIGNED_INT, 0, this->_nbParticles);
	glBindVertexArray(0);
}

void							Mesh::updateMesh(float t)
{
	if (!this->_dynamic)
		return ;

	std::vector<t_vec>			tops;
	GRID						map(this->_gridSize, std::vector<float>(this->_gridSize));


	createMap(map, this->_gridSize, this->_maxHeight, tops, t);
	this->_vertexOO.clear();
	this->_vertexNOO.clear();
	computeVert(map, this->_gridSize);
	computeNorm();

	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(t_vec) * this->_vertexOO.size(), &this->_vertexOO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(t_vec) * this->_vertexNOO.size(), &this->_vertexNOO[0]);
	glBindVertexArray(0);
}
 
void							Mesh::createTerrain(bool top)
{
	std::vector<t_vec>			tops;
	GRID						map(this->_gridSize, std::vector<float>(this->_gridSize));
	t_vec						t1 = {25, 25, 25};
	t_vec						t2 = {50, 50, 50};
	t_vec						t3 = {75, 75, 75};
	ProcMap						pMap(this->_gridSize);

	if (top)
	{
		tops.push_back(t1);
		tops.push_back(t2);
		tops.push_back(t3);
	}
	
//	createMap(map, this->_gridSize, this->_maxHeight, tops, 0.7f);
	pMap.createMap();
	pMap.scaleMap(this->_maxHeight);
	map = pMap.getMap();
	computeVert(map, this->_gridSize);
	computeNorm();
	computeIndices();
}

OpenGLMatrix*					Mesh::getModelMatrix( void ) { return &(this->_modelMatrix); }
void							Mesh::setModelMatrix( OpenGLMatrix newMatrix ) { this->_modelMatrix = newMatrix; }
std::vector< t_vec >			Mesh::getPosition() const { return this->_vertexOO; }
GLuint       					*Mesh::getVBO( void ) { return this->_vbo; }
void                   			Mesh::setNbParticles( int n ) { this->_nbParticles = n; }

std::ostream&					operator<<(std::ostream& flux, Mesh const& m)
{
	std::vector< t_vec >		v = m.getPosition();

	flux << std::fixed << std::setprecision(5);
	flux << "Mesh Position:" << std::endl;
	for (const auto& it : v)
	{
		flux << std::setw(10) << it.x << " " << std::setw(10) << it.y << std::setw(10) << it.z << " ";
	}
	flux << std::endl << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return flux;
}

/*	******************************
 *	******************************
 * 		  PARSING OBJ FILE
 *	******************************
 *	******************************
 */

/*
void				Mesh::getFace(std::string face)
{
	GLuint				v_i;
	GLuint				v_n;
	std::string			p;
	std::istringstream iss(face);

	iss >> v_i >> p >> v_n >> p >> v_n;
	_indices.push_back(v_i - 1.0f);
}
*/
void				printV(std::vector< t_vec > v)
{
	std::cout << std::endl;
	for (size_t i = 0 ; i < v.size() ; i++)
	{
		std::cout << i << ":\t" << v[i].x << " " << v[i].y << " " << v[i].z << std::endl;
	}
	std::cout << std::endl;
}

void				printV(std::vector< GLuint > v)
{
	std::cout << std::endl;
	for (size_t i = 0 ; i < v.size() ; i++)
	{
		std::cout << v[i] ;
		if ( (i + 1) % 3 == 0)
			std::cout << "\n";
		else
			std::cout << "\t";
	}
	std::cout << std::endl;
}

void				printT(double t)
{
	int				m, s, ms;

	m = (int)(t / 60000);
	s = (int)(t / 1000) % 60;
	ms = (int)t % 1000; 
	std::cout << "Parsing time: " << m << "m" << s << "s" << ms << "ms." << std::endl;
}

int					isDigit(char c)
{
	return ( c >= '0' && c <= '9' );
}

GLuint				retrieveNb(std::string str, int *i)
{
	std::string		tmp;
	while (str[*i] != '\0' && isDigit(str[*i]))
	{
		tmp += str[*i];
		*i = *i + 1;
	}
	return std::stoi(tmp);
}

void				retrieveInd(std::string face, GLuint &v_i, GLuint &vt_i, GLuint &vn_i)
{
	int				i = 0;

	v_i = retrieveNb(face, &i);
	if (face[i] == '/')
	{
		i++;
		if (face[i] == '/')
		{
			i++;
			vt_i = 0;
			vn_i = retrieveNb(face, &i);
		}
		else if (isDigit(face[i]))
		{
			vt_i = retrieveNb(face, &i);
			if (face[i] == '/')
			{
				i++;
				vn_i = retrieveNb(face, &i);
			}
			else
				vn_i = 0;
		}
	}
	else 
	{
		vt_i = 0;
		vn_i = 0;
	}
}

t_vec				normalize(t_vec v)
{
	t_vec			v2;
	int				len;

	len = v.x * v.x + v.y * v.y + v.z * v.z;
	len = sqrt(len);
	v2.x = v.x / len;
	v2.y = v.y / len;
	v2.z = v.z / len;
	return v2;
}

t_vec				addVec(t_vec v1, t_vec v2)
{
	t_vec			retV;

	retV.x = v1.x + v2.x;
	retV.y = v1.y + v2.y;
	retV.z = v1.z + v2.z;
	return retV;
}

void				Mesh::getFace(std::string face)
{
	GLuint				v_i;
	GLuint				v_n;
	GLuint				v_t;
	std::string			p;
	std::istringstream iss(face);

	auto it = _triplets.find(face);
	if (it != _triplets.end())
	{
		_indicesOO.push_back(_triplets[face]);
		iss >> v_i >> p >> v_n >> p >> v_n;
		_indices.push_back(v_i - 1.0f);
	}
	else
	{
//		iss >> v_i >> p >> v_n >> p >> v_n;
		retrieveInd(face, v_i, v_t, v_n);
		_vertexOO.push_back(_vertex[v_i - 1.0f]);
		_vertexNOO.push_back(_vertexN[v_n - 1.0f]);
		_indicesOO.push_back(_curInd);
		_triplets[face] = _curInd;
		_curInd++;
		_indices.push_back(v_i - 1.0f);
	
	_vecIds.push_back(v_i - 1.0f);
	auto it2 = _oneNorm.find(v_i - 1.0f);
	if (it2 != _oneNorm.end())
		_oneNorm[v_i - 1.0f] = normalize(addVec(_oneNorm[v_i - 1.0f], _vertexN[v_n - 1.0f]));
	else
		_oneNorm[v_i - 1.0f] = _vertexN[v_n - 1.0f];
	
	}
}

void				Mesh::loadOBJ(const std::string fileName)
{
	std::ifstream	infile(fileName);
	std::string		line;
	double			last = glfwGetTime();

	if (!infile.good())
		std::cout << "no file" << std::endl;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string			token;
		iss >> token;
		if (token == "v")
		{
			t_vec		v;
			iss >> v.x >> v.y >> v.z;
			_vertex.push_back(v);
		}
		else if (token == "vt")
		{
			t_vec2		v;
			iss >> v.u >> v.v;
			_vertexT.push_back(v);
		}
		else if (token == "vn")
		{
			t_vec		v;
			iss >> v.x >> v.y >> v.z;
			_vertexN.push_back(v);
		}
		else if (token == "f")
		{
			std::string		f_v;
			iss >> f_v;
			getFace(f_v);	
			iss >> f_v;
			getFace(f_v);	
			iss >> f_v;
			getFace(f_v);	
		}
	 }
/*
	std::cout << "vertex: "  ; printV(_vertex) ; std::cout << std::endl;
	std::cout << "vertexN: " ; printV(_vertexN) ; std::cout << std::endl;
	std::cout << "indices: " ; printV(_indices) ; std::cout << std::endl;
	std::cout << "#########" << std::endl;
	std::cout << "vertexOO: " ; printV(_vertexOO) ; std::cout << std::endl;
	std::cout << "vertexNOO: " ; printV(_vertexNOO) ; std::cout << std::endl;
	std::cout << "indicesOO: " ; printV(_indicesOO) ; std::cout << std::endl;
*/
//	std::cout << "Parsing Time: " << glfwGetTime() - last << "s." << std::endl;
	rearrange();
	printT((glfwGetTime() - last) * 1000);
}

void				Mesh::rearrange()
{
	size_t			i = 0;

	while (i < _vertexOO.size())
	{
		_vertexNOO[i] = _oneNorm[_vecIds[i]];	
		i++;
	}
	
}

/*	******************************
 *	******************************
 * 		END PARSING OBJ FILE
 *	******************************
 *	******************************
 */

/*	******************************
 *	******************************
 *	      GENERATE GEOMETRY
 *	******************************
 *	******************************
 */

float					Mesh::calcUp(int i, int j, t_vec top)
{
	float		n;

	n = sqrt(pow(static_cast<float>(i) - top.x, 2.0) + pow(static_cast<float>(j) - top.y, 2.0));
	n = n / THRESHOLD;
	if (n > 1.0f)
		n = 1.0f;
	n = 1 - n;
//	printf("n: %f\n=================\n", n);
	return (n);
}

void					Mesh::createMap(GRID &map, int gridSize, float height, std::vector<t_vec> tops, float t)
{
	int					octaves	= 6;
	float				n = 0, m, b;
	float				nCoef = 0.f;

	for (int i = 0 ; i < gridSize ; i++)
	{
		for (int j = 0 ; j < gridSize ; j++)
		{
			n = 0.0f;
			for (size_t it = 0 ; it < tops.size() ; it++) {
				m = calcUp(i, j, tops[it]);
				nCoef = m * 0.5f;
				if (m < 0.1)
					m = 0.1;
//				nCoef = m + 0.5;
				b = Noise::sound3D(i, j, t, octaves, 0.5);
//				m = b * nCoef + sin(m * M_PI_2) * sin(m * M_PI_2);
				m = b + nCoef;
				n = fmax(n, m);
			}
			if (tops.size() == 0)
				map[i][j] = Noise::sound3D(i, j, t, octaves, 0.5) * height;
			else
				map[i][j] = n * height;
		}
	}
}
 
void					Mesh::computeVert(GRID &map, int gridSize)
{
	for (int i = 0 ; i < gridSize ; i++)
	{
		for (int j = 0 ; j < gridSize ; j++)
		{
			t_vec v = {static_cast<GLfloat>(j - gridSize/2), map[i][j], static_cast<GLfloat>(gridSize/2 - i)};
			this->_vertexOO.push_back(v);
		}
	}
}

void					Mesh::computeIndices()
{
	for (int i = 0 ; i < this->_gridSize - 1 ; i++)
	{
		for (int j = 0 ; j < this->_gridSize - 1 ; j++)
		{
			this->_indicesOO.push_back(i * this->_gridSize + j);
			this->_indicesOO.push_back(i * this->_gridSize + j + 1);
			this->_indicesOO.push_back((i + 1) * this->_gridSize + j);
			this->_indicesOO.push_back((i + 1) * this->_gridSize + j);
			this->_indicesOO.push_back((i + 1) * this->_gridSize + j + 1);
			this->_indicesOO.push_back(i * this->_gridSize + j + 1);
		}
	}
}

t_vec				cross(t_vec v1, t_vec v2)
{
	t_vec			res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return res;
}

t_vec				add(t_vec v1, t_vec v2)
{
	t_vec			res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return res;
}

t_vec				sub(t_vec v1, t_vec v2)
{
	t_vec			res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return res;
}

t_vec					Mesh::getNorm(GLuint center, GLuint first, GLuint second)
{
	t_vec				v1;
	t_vec				v2;
	t_vec				vres;

	v1 = sub(this->_vertexOO[first], this->_vertexOO[center]);
	v2 = sub(this->_vertexOO[second], this->_vertexOO[center]);
	vres = normalize(cross(v1, v2));
	return vres;
}

void					Mesh::computeNorm()
{
	for (int i = 0 ; i < this->_gridSize ; i++)
	{
		for (int j = 0 ; j < this->_gridSize ; j++)
		{
//			t_vec d_r = normalize(cross(sub(map[i][j+1], map[i][j]), sub(map[i+1][j], map[i][j])));
//			t_vec d_l = normalize(cross(sub(map[i+1][j], map[i][j]), sub(map[i][j-1], map[i][j])));
//			t_vec u_l = normalize(cross(sub(map[i][j-1], map[i][j]), sub(map[i-1][j], map[i][j])));
//			t_vec u_r = normalize(cross(sub(map[i-1][j], map[i][j]), sub(map[i][j+1], map[i][j])));
			if (i > 0 && i < this->_gridSize - 1 && j > 0 && j < this->_gridSize - 1)
			{
				t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
				t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
				this->_vertexNOO.push_back(normalize(add(d_r, u_l)));
			}

			else if (i == 0)
			{
				if (j == 0)
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					this->_vertexNOO.push_back(normalize(d_r));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					this->_vertexNOO.push_back(normalize(d_l));
				}
				else
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					this->_vertexNOO.push_back(normalize(add(d_r, d_l)));
				}
			}

			else if (i == this->_gridSize - 1)
			{
				if (j == 0)
				{
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexNOO.push_back(normalize(u_r));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					this->_vertexNOO.push_back(normalize(u_l));
				}
				else
				{
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexNOO.push_back(normalize(add(u_l, u_r)));
				}
			}

			else if (i > 0 && i < this->_gridSize - 1)
			{
				if (j == 0)
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexNOO.push_back(normalize(add(u_r, d_r)));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					this->_vertexNOO.push_back(normalize(add(d_l, u_l)));
				}
			}


		}
	}
//			printV(this->_vertexNOO);
}

/*	******************************
 *	******************************
 *	   END GENERATE GEOMETRY
 *	******************************
 *	******************************
 */

