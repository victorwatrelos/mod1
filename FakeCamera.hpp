#ifndef FAKECAMERA_HPP
# define FAKECAMERA_HPP

# include "CameraControl.hpp"

class				FakeCamera : public CameraControl
{
	public:
					FakeCamera(float x, float y, float z);
		void		controlKey(int key, int action, int mods);
		void		controlMouse(double xPos, double yPos);

	protected:
		float		_x;
		float		_y;
		float		_z;
};

#endif
