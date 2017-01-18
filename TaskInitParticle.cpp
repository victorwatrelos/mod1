#include "TaskInitParticle.hpp"

TaskInitParticle::TaskInitParticle(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskInitParticle::createKernel() {
    this->_createKernel("kernel/initParticle.cl", "initParticle");
}

cl_int  TaskInitParticle::getMaxGid() {
    return this->_maxGid;
}
