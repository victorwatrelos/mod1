#ifndef FREECAMERA_HPP
# define FREECAMERA_HPP

# include "CameraControl.hpp"

class				FreeCamera : public CameraControl
{
	public:
					FreeCamera(t_vecf eyePos, float pitch, float yaw);
		void		controlKey(int key, int action, int mods);
		void		controlMouse(double xPos, double yPos);

	private:
		Matrix		calculateMatrix( void );
		void		calcViewDir( void );
		t_vecf		mergeKeyPress( float sens );
		void		initKeyPress( void );

	protected:
		t_vecf		_eyePos;
		float		_pitch;
		float		_yaw;
		t_vecf		_keyPress[3];
		t_vecf		_viewDir;
		t_vecf		_viewNorm;
		double		_oldX;
		double		_oldY;
};

#endif
