#ifndef LOOKATCAMERA_HPP
# define LOOKATCAMERA_HPP

# include "CameraControl.hpp"

class				LookAtCamera : public CameraControl
{
	public:
					LookAtCamera(t_vecf eyePos, t_vecf targetPos, t_vecf upVector);
		void		controlKey(int key, int action, int mods);
		void		controlMouse(double xPos, double yPos);

	private:
		Matrix		calculateMatrix();

	protected:
		t_vecf		_eyePos;
		t_vecf		_targetPos;
		t_vecf		_upVector;
};

#endif
