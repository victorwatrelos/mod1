#include "LookAtCamera.hpp"

LookAtCamera::LookAtCamera(t_vecf eyePos, t_vecf targetPos, t_vecf upVector)
	: _eyePos(eyePos), _targetPos(targetPos), _upVector(upVector)
{
	this->_viewMatrix.setMatrix(calculateMatrix());
}

void			LookAtCamera::controlKey(int key, int action, int mods)
{
	float		sens = 0.5f;

	if (action == ACTION_PRESS)
	{
		if (mods == MOD_SHIFT)
		{
			if (key == 32) // KEY_SPACE
				this->_eyePos.z -= sens;
		}
		else
		{
			if (key == 32) // KEY_SPACE
				this->_eyePos.z += sens;
			else if (key == (int)'W')
				this->_eyePos.y += sens;
			else if (key == (int)'S')
				this->_eyePos.y -= sens;
			else if (key == (int)'D')
				this->_eyePos.x += sens;
			else if (key == (int)'A')
				this->_eyePos.x -= sens;
		}
	}
	this->_viewMatrix.setMatrix(calculateMatrix());
}

void		LookAtCamera::controlMouse(double xPos, double yPos)
{
	(void)xPos;
	(void)yPos;
}

Matrix			LookAtCamera::calculateMatrix()
{
	t_vecf zAxis = normalize(sub(this->_targetPos, this->_eyePos));
	t_vecf xAxis = normalize(cross(this->_upVector, zAxis));
	t_vecf yAxis = cross(zAxis, xAxis);

	Matrix		viewMatrix(xAxis.x, xAxis.y, xAxis.z, 0,
						   yAxis.x, yAxis.y, yAxis.z, 0,
						   zAxis.x, zAxis.y, zAxis.z, 0,
						  -dot( xAxis, _eyePos ), -dot( yAxis, _eyePos ), dot( zAxis, _eyePos ), 1);
	return viewMatrix;
}
