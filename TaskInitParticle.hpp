#ifndef TASKINITPARTICLE
# define TASKINITPARTICLE

# include "Task.hpp"

class   TaskInitParticle: public Task {
public:
    TaskInitParticle(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
};
#endif
