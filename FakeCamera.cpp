#include "FakeCamera.hpp"

FakeCamera::FakeCamera(float x, float y, float z) : _x(x), _y(y), _z(z)
{
	this->_viewMatrix.translate(this->_x, this->_y, this->_z);
}

void			FakeCamera::controlKey(int key, int action, int mods)
{
	float		sens = 1.0f;

	if (action == ACTION_PRESS)
	{
		if (mods == MOD_SHIFT)
		{
			if (key == 32) // KEY_SPACE
				this->_y -= sens;
		}
		else
		{
			if (key == 32) // KEY_SPACE
				this->_y += sens;
			else if (key == (int)'W')
				this->_z += sens;
			else if (key == (int)'S')
				this->_z -= sens;
			else if (key == (int)'D')
				this->_x += sens;
			else if (key == (int)'A')
				this->_x -= sens;
		}
	}
	this->_viewMatrix.resetMatrix();
	this->_viewMatrix.translate(this->_x, this->_y, this->_z);
}

void		FakeCamera::controlMouse(double xPos, double yPos)
{
	(void)xPos;
	(void)yPos;
}
