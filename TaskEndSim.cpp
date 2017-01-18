#include "TaskEndSim.hpp"

TaskEndSim::TaskEndSim(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskEndSim::createKernel() {
    this->_createKernel("kernel/endSim.cl", "endSim");
}

cl_int  TaskEndSim::getMaxGid() {
    return this->_maxGid;
}
