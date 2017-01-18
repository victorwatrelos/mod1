#include "Top.hpp"

Top::Top( double pHigh, double pXCenter, double pYCenter )
	: high(pHigh), xCenter(pXCenter), yCenter(pYCenter)
{
}

void	Top::printTop()
{
	std::cout << "high: " << this->high << "x: " << this->xCenter << " y: " << this->yCenter << std::endl;
}
