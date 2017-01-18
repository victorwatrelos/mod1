#include "TaskApplyForces.hpp"

TaskApplyForces::TaskApplyForces(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskApplyForces::createKernel() {
    this->_createKernel("kernel/applyForces.cl", "applyForces");
}

cl_int  TaskApplyForces::getMaxGid() {
    return this->_maxGid;
}
