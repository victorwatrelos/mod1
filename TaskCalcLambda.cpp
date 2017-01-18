#include "TaskCalcLambda.hpp"

TaskCalcLambda::TaskCalcLambda(
        cl_context context,
        cl_device_id device,
        cl_int nbParticle)
{
    this->_context = context;
    this->_device = device;
    this->_nbParticle = nbParticle;
}

void        TaskCalcLambda::createKernel() {
    this->_createKernel("kernel/calcLambda.cl", "calcLambda");
}

cl_int  TaskCalcLambda::getMaxGid() {
    return this->_maxGid;
}
