#include "TaskAddConst.hpp"

TaskAddConst::TaskAddConst(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskAddConst::createKernel() {
    this->_createKernel("kernel/addConst.cl", "addConst");
}

cl_int  TaskAddConst::getMaxGid() {
    return this->_maxGid;
}
