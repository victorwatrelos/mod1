#ifndef TASKADDCONST
# define TASKADDCONST

# include "Task.hpp"

class   TaskAddConst : public Task {
public:
    TaskAddConst(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
};
#endif
