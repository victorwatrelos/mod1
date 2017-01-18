#ifndef TASKENDSIM_HPP
# define TASKENDSIM_HPP

# include "Task.hpp"

class   TaskEndSim: public Task {
public:
    TaskEndSim(cl_context context, cl_device_id device, cl_int nbParticle);
    void        createKernel();
    cl_int      getMaxGid();
};

#endif
