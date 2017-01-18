#ifndef TASK_HPP
# define TASK_HPP

# include <OpenCL/cl.h>
# include <string>
# include <fstream>
# include "clUtil.hpp"
class Task {
public:
    cl_kernel           getKernel();
    std::string         *getSrc(std::string filename) const;
    void                enqueueKernel(cl_command_queue queue);
    void                releaseKernel();
protected:
    void            _createKernel(std::string filename, std::string kernelName);
    cl_kernel       _kernel;
    cl_program      _program;
    cl_context      _context;
    cl_device_id    _device;
    size_t      _localWorkSize;
    size_t      _globalWorkSize;
    size_t      _nbWorkGroup;
    size_t      *_maxWorkItemSize;
    cl_int      _nbParticle;
    cl_int      _maxGid;
};

#endif
