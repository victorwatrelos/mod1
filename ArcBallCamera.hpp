#ifndef LOOKATCAMERA_HPP
# define LOOKATCAMERA_HPP

# include "CameraControl.hpp"

class				LookAtCamera : public CameraControl
{
	public:
					LookAtCamera();
		void		control(int key, int action, int mods);
};

#endif
