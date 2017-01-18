#ifndef TOP_HPP
# define TOP_HPP

#include <iostream>

class Top
{
	public:
		Top( double high, double xCenter, double yCenter );
		double const	high;
		double const	xCenter;
		double const	yCenter;
		void			printTop();
	private:
};

#endif
