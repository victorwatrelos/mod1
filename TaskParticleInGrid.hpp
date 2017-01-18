#ifndef TASKPARTICLEINGRID
# define TASKPARTICLEINGRID

# include "Task.hpp"

class TaskParticleInGrid : public Task {

public:
    TaskParticleInGrid(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
private:
};

#endif
