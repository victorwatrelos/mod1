#ifndef TASKAPPLYFORCES
# define TASKAPPLYFORCES

# include "Task.hpp"

class   TaskApplyForces : public Task {
public:
    TaskApplyForces(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
private:
};
#endif
