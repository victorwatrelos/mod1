#ifndef TASKCALCLAMBDA
# define TASKCALCLAMBDA

# include "Task.hpp"

class TaskCalcLambda : public Task {
public:
    TaskCalcLambda(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
};


#endif
