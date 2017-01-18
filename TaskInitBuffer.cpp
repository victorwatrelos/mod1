#include "TaskInitBuffer.hpp"

TaskInitBuffer::TaskInitBuffer(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskInitBuffer::createKernel(int sizeGrid) {
    this->_nbParticle = sizeGrid;
    this->_createKernel("kernel/initBuffer.cl", "initBuffer");
}

cl_int  TaskInitBuffer::getMaxGid() {
    return this->_maxGid;
}
