#include "TaskFindNeighbors.hpp"

TaskFindNeighbors::TaskFindNeighbors(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskFindNeighbors::createKernel() {
    this->_createKernel("kernel/findNeighbors.cl", "findNeighbors");
}

cl_int  TaskFindNeighbors::getMaxGid() {
    return this->_maxGid;
}
