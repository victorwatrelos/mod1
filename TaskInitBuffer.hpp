#ifndef TASKINITBUFFER
# define TASKINITBUFFER

# include "Task.hpp"

class TaskInitBuffer : public Task {
public:
    TaskInitBuffer(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel(int sizeGrid);
    cl_int      getMaxGid();
};

#endif
