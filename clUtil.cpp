#include "clUtil.hpp"

void    checkCLSuccess(cl_int errNum, std::string name) {
    if (errNum != CL_SUCCESS) {
        std::cout << "OpenCL fail on " << name << std::endl;
        throw new OpenCLException();
    }
}
