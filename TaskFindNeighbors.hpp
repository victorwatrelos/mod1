#ifndef TASKFINDNEIGHBORS
# define TASKFINDNEIGHBORS

# include "Task.hpp"

class TaskFindNeighbors : public Task {
public:
    TaskFindNeighbors(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
};


#endif
