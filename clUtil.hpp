#ifndef CLUTIL_HPP
# define CLUTIL_HPP

# include <string>
# include <OpenCL/cl.h>
# include <iostream>
# include "OpenCLException.hpp"

void    checkCLSuccess(cl_int errNum, std::string name);

#endif
