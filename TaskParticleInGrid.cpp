#include "TaskParticleInGrid.hpp"

TaskParticleInGrid::TaskParticleInGrid(cl_context context, cl_device_id device, cl_int nbParticle)
{
    this->_nbParticle = nbParticle;
    this->_context = context;
    this->_device = device;
}

void        TaskParticleInGrid::createKernel() {
    this->_createKernel("kernel/particleInGrid.cl", "particleInGrid");
}

cl_int  TaskParticleInGrid::getMaxGid() {
    return this->_maxGid;
}
