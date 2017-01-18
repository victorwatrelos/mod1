#ifndef OPENCL_HPP
# define OPENCL_HPP

# define GLFW_EXPOSE_NATIVE_COCOA
# define GLFW_EXPOSE_NATIVE_NSGL

# include <string>
# include <iostream>
# include <sys/time.h>
# include <OpenCL/cl.h>
# include <OpenCL/cl_gl.h>
# include <OpenCL/cl_gl_ext.h>
# include <OpenGL/OpenGL.h>
# include <fstream>
# include <cmath>
# include "Platform.hpp"
# include "TaskParticleInGrid.hpp"
# include "TaskApplyForces.hpp"
# include "TaskAddConst.hpp"
# include "TaskInitBuffer.hpp"
# include "TaskInitParticle.hpp"
# include "TaskCalcLambda.hpp"
# include "TaskEndSim.hpp"
# include "TaskFindNeighbors.hpp"

class OpenCL {
    public:
        const size_t    X = 0;
        const size_t    Y = 1;
        const size_t    Z = 2;
        OpenCL(cl_int nbParticles, cl_float sizeGridCoef, cl_int maxParticlesPerCell, cl_int gridX, cl_int gridY, cl_int gridZ);
        virtual ~OpenCL( void );
        void            initOpenCL(GLuint vbo);
        void            executeKernel();
        void            release();
        static void     displayInformation();
    private:
        void           _createContext();
        void           _bindBuffer();
        void           _getDeviceInfo();
        void           _createCommandQueue( void );
        void           _setKernelArg();
        void           _initTask();
        void           _setStdArg(cl_kernel kernel);
        void           _setKernelConstArg(cl_kernel kernel);
        void            _bindVBO(GLuint vbo);

        Platform                    _platform;
        cl_device_id                _device;
        cl_int                      _nbDevice;
        cl_context                  _context;
        cl_command_queue            _commandQueue;
        cl_uint                     _maxWorkingDimensions;
        cl_int                      _maxGid;
        cl_mem                      _particles;
        cl_mem                      _particlesVBO;
        cl_mem                      _particlesIdByCells;
        cl_mem                      _particlesVelocity;
        cl_mem                      _particlesProjection;
        cl_mem                      _particlesLambda;
        cl_mem                      _particlesNeighbors;
        size_t                      _sizeGrid;

        cl_int                      _nbParticles;
        cl_int                      _gridSize[3];
        cl_float                    _sizeGridCoef;
        cl_int                      _maxParticlesPerCell;
        TaskParticleInGrid          *_taskParticlesInGrid = NULL;
        TaskApplyForces             *_taskApplyForces = NULL;
        TaskAddConst                *_taskAddConst = NULL;
        TaskInitBuffer              *_taskInitBuffer = NULL;
        TaskInitParticle            *_taskInitParticles = NULL;
        TaskCalcLambda              *_taskCalcLambda = NULL;
        TaskFindNeighbors           *_taskFindNeighbors = NULL;
        TaskEndSim                  *_taskEndSim = NULL;
        size_t                      *_maxWorkItemSize;
};

#endif
