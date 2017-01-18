#include "OpenCL.hpp"
#define NB_TRY 1

OpenCL::OpenCL(cl_int nbParticles, cl_float sizeGridCoef, cl_int maxParticlesPerCell, cl_int gridX, cl_int gridY, cl_int gridZ) 
: _nbParticles(nbParticles), _sizeGridCoef(sizeGridCoef), _maxParticlesPerCell(maxParticlesPerCell), _maxWorkItemSize(NULL)
{
    this->_gridSize[OpenCL::X] = gridX;
    this->_gridSize[OpenCL::Y] = gridY;
    this->_gridSize[OpenCL::Z] = gridZ;
    this->_maxGid = this->_nbParticles - 1;
}

void    OpenCL::_initTask() {
    this->_taskParticlesInGrid = new TaskParticleInGrid(this->_context, this->_device, this->_nbParticles);
    this->_taskParticlesInGrid->createKernel();

    this->_taskApplyForces = new TaskApplyForces(this->_context, this->_device, this->_nbParticles);
    this->_taskApplyForces->createKernel();

    this->_taskAddConst = new TaskAddConst(this->_context, this->_device, this->_nbParticles);
    this->_taskAddConst->createKernel();


    this->_taskInitBuffer = new TaskInitBuffer(this->_context, this->_device, this->_nbParticles);
    this->_taskInitBuffer->createKernel(this->_sizeGrid);

    this->_taskInitParticles = new TaskInitParticle(this->_context, this->_device, this->_nbParticles);
    this->_taskInitParticles->createKernel();

    this->_taskEndSim = new TaskEndSim(this->_context, this->_device, this->_nbParticles);
    this->_taskEndSim->createKernel();

    this->_taskCalcLambda = new TaskCalcLambda(this->_context, this->_device, this->_nbParticles);
    this->_taskCalcLambda->createKernel();

    this->_taskFindNeighbors= new TaskFindNeighbors(this->_context, this->_device, this->_nbParticles);
    this->_taskFindNeighbors->createKernel();
}

void    OpenCL::initOpenCL(GLuint vbo) {
    this->_createContext();
    this->_getDeviceInfo();
    this->_bindBuffer();
    this->_bindVBO(vbo);
    this->_initTask();
    this->_createCommandQueue();
    this->_setKernelArg();
    this->_taskInitParticles->enqueueKernel(this->_commandQueue);
    clFinish(this->_commandQueue);
}

void    OpenCL::_createContext() {
    cl_int      err;

    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

    cl_context_properties properties[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)kCGLShareGroup,
        0
    };

    this->_context = clCreateContext(properties, 0, 0, NULL, NULL, &err);
    checkCLSuccess(err, "clCreateContext");
    checkCLSuccess(clGetContextInfo(this->_context,
                CL_CONTEXT_NUM_DEVICES,
                sizeof(cl_int),
                &(this->_nbDevice),
                NULL),
        "clGetContextInfo");

    if (this->_nbDevice < 1) {
        throw new OpenCLException();
    }

    checkCLSuccess(clGetContextInfo(this->_context,
                CL_CONTEXT_DEVICES,
                sizeof(cl_device_id),
                &(this->_device),
                NULL),
        "clGetContextInfo");
}

void    OpenCL::_getDeviceInfo() {
    size_t      size;
    char        *deviceName;

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_NAME,
                0,
                NULL,
                &size),
        "clGetDeviceInfo size");
    deviceName = new char[size];
    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_NAME,
                size,
                deviceName,
                NULL),
            "clGetDeviceInfo info");
    std::cout << "Device: " << deviceName << std::endl;
    delete[] deviceName;

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                sizeof(cl_uint),
                &(this->_maxWorkingDimensions),
                NULL),
            "clGetDeviceInfo max working dimensions");

    this->_maxWorkItemSize = new size_t[this->_maxWorkingDimensions];

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_MAX_WORK_ITEM_SIZES,
                sizeof(size_t) * this->_maxWorkingDimensions,
                this->_maxWorkItemSize,
                NULL),
            "clGetDeviceInfo max working dimensions");

    std::cout << "(";
    for (size_t i = 0; i < this->_maxWorkingDimensions; i++) {
        std::cout << this->_maxWorkItemSize[i] << ", ";
    }
    std::cout << ")" << std::endl;

}

void    OpenCL::_setStdArg(cl_kernel kernel) {
    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particles),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_mem), &this->_particlesVelocity),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 2, sizeof(cl_mem), &this->_particlesProjection),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 3, sizeof(cl_mem), &this->_particlesIdByCells),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 4, sizeof(cl_float), &this->_sizeGridCoef),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 5, sizeof(cl_int), &this->_gridSize[OpenCL::X]),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 6, sizeof(cl_int), &this->_gridSize[OpenCL::Y]),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 7, sizeof(cl_int), &this->_gridSize[OpenCL::Z]),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 8, sizeof(cl_int), &this->_maxParticlesPerCell),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 9, sizeof(cl_int), &this->_maxGid),
            "clSetKernelArg");
}

void    OpenCL::_setKernelConstArg(cl_kernel kernel) {
    checkCLSuccess(clSetKernelArg(kernel, 10, sizeof(cl_mem), &this->_particlesLambda),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 11, sizeof(cl_mem), &this->_particlesNeighbors),
            "clSetKernelArg");
}

void    OpenCL::_setKernelArg() {
    cl_kernel       kernel;

    this->_setStdArg(this->_taskParticlesInGrid->getKernel());
    this->_setStdArg(this->_taskApplyForces->getKernel());
    this->_setStdArg(this->_taskAddConst->getKernel());
    this->_setStdArg(this->_taskEndSim->getKernel());
    this->_setStdArg(this->_taskCalcLambda->getKernel());
    this->_setStdArg(this->_taskFindNeighbors->getKernel());
    this->_setKernelConstArg(this->_taskAddConst->getKernel());
    this->_setKernelConstArg(this->_taskCalcLambda->getKernel());
    this->_setKernelConstArg(this->_taskFindNeighbors->getKernel());

    kernel = this->_taskInitBuffer->getKernel();

    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particlesIdByCells),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_int), &this->_sizeGrid),
            "clSetKernelArg");

    kernel = this->_taskInitParticles->getKernel();

    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particles),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_mem), &this->_particlesVelocity),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 2, sizeof(cl_int), &this->_maxGid),
            "clSetKernelArg");

    kernel = this->_taskEndSim->getKernel();
    checkCLSuccess(clSetKernelArg(kernel, 10, sizeof(cl_mem), &this->_particlesVBO),
            "clSetKernelArg");

}

void    OpenCL::executeKernel() {
    int     err;

    struct timeval      timeVal1;
    struct timeval      timeVal2;
    unsigned long long int  start;
    unsigned long long int  end;
    double                  time;
    unsigned long long int  time2;
    static cl_float         wallStep = 1000.0f;
    static cl_float         step = 20.0f;
    cl_kernel               kernel;

    kernel = this->_taskAddConst->getKernel();
    checkCLSuccess(clSetKernelArg(kernel, 12, sizeof(cl_float), &wallStep),
            "clSetKernelArg");

    err = clEnqueueAcquireGLObjects(
        this->_commandQueue,
        1,
        &this->_particlesVBO,
        0,
        NULL,
        NULL);
    checkCLSuccess(err, "clEnqueueAcquireGLObjects");

    gettimeofday(&timeVal1, NULL);

    for ( int j = 0; j < NB_TRY; j++) {
        this->_taskInitBuffer->enqueueKernel(this->_commandQueue);
        this->_taskParticlesInGrid->enqueueKernel(this->_commandQueue);
        this->_taskApplyForces->enqueueKernel(this->_commandQueue);
        this->_taskFindNeighbors->enqueueKernel(this->_commandQueue);
        for (int i = 0; i < 3; i++) {
            this->_taskCalcLambda->enqueueKernel(this->_commandQueue);
            this->_taskAddConst->enqueueKernel(this->_commandQueue);
        }
        this->_taskEndSim->enqueueKernel(this->_commandQueue);
    }
    clFinish(this->_commandQueue);
    gettimeofday(&timeVal2, NULL);
    start = 1000000 * timeVal1.tv_sec + (timeVal1.tv_usec);
    end = 1000000 * timeVal2.tv_sec + (timeVal2.tv_usec);
    time = ((double)(end - start)) / 1000000.0;
    time2 = (end - start) / NB_TRY;
// printf("Execution time: %llu ms, %llu us\n", time2 / 1000, time2);


    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_particlesVBO,
            0,
            NULL,
            NULL);
    clFinish(this->_commandQueue);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
    wallStep += step;
    if (wallStep > 2000.0f) {
        if (step > 0.0f) {
            step = -100.0f;
        }
    }
    if (wallStep < std::fabs(step)) {
        if (step < 0.0f) {
            step = 10.0f;
        }
    }
}

void    OpenCL::release() {
    std::cout << "OpenCL ressources release" << std::endl;
    clFinish(this->_commandQueue);
    clReleaseMemObject(this->_particles);
    clReleaseMemObject(this->_particlesIdByCells);
    clReleaseMemObject(this->_particlesVelocity);
    clReleaseMemObject(this->_particlesProjection);
    clReleaseMemObject(this->_particlesLambda);
    clReleaseMemObject(this->_particlesNeighbors);

    this->_taskParticlesInGrid->releaseKernel();
    this->_taskApplyForces->releaseKernel();
    this->_taskAddConst->releaseKernel();
    this->_taskInitBuffer->releaseKernel();
    this->_taskInitParticles->releaseKernel();
    this->_taskCalcLambda->releaseKernel();
    this->_taskFindNeighbors->releaseKernel();
    this->_taskEndSim->releaseKernel();

    clReleaseCommandQueue(this->_commandQueue);
    clReleaseContext(this->_context);
    std::cout << "DONE" << std::endl;
}

void    OpenCL::_bindBuffer() {
    int     err;
    size_t  sizeParticlesBuffer = this->_nbParticles * 4 * sizeof(cl_float);

    this->_sizeGrid = this->_gridSize[OpenCL::X] * this->_gridSize[OpenCL::Y] * this->_gridSize[OpenCL::Z] * (this->_maxParticlesPerCell + 1);
    std::cout << "size: " << this->_sizeGrid << std::endl;
    this->_particlesIdByCells = clCreateBuffer(this->_context,
            CL_MEM_READ_WRITE, this->_sizeGrid * sizeof(cl_int),
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer id by cell");

    std::cout << "Buffer size: " << sizeParticlesBuffer << std::endl;
    this->_particles = clCreateBuffer(
            this->_context,
            CL_MEM_READ_WRITE,
            sizeParticlesBuffer,
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer particle velocity");

    this->_particlesVelocity = clCreateBuffer(this->_context,
            CL_MEM_READ_WRITE,
            sizeParticlesBuffer,
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer particle velocity");

    this->_particlesProjection = clCreateBuffer(
            this->_context,
            CL_MEM_READ_WRITE,
            sizeParticlesBuffer,
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer projection");

    this->_particlesLambda = clCreateBuffer(
            this->_context,
            CL_MEM_READ_WRITE,
            this->_nbParticles * sizeof(cl_float),
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer projection");

    this->_particlesNeighbors = clCreateBuffer(
            this->_context,
            CL_MEM_READ_WRITE,
            this->_nbParticles * sizeof(cl_int) * 200,
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer projection");
}


void    OpenCL::_bindVBO(GLuint vbo) {
    cl_int     err;

    std::cout << "vbo: " << vbo << std::endl;
    this->_particlesVBO = clCreateFromGLBuffer(
            this->_context,
            CL_MEM_READ_WRITE,
            vbo,
            &err);
    checkCLSuccess(err, "clCreateFromGLBuffer");
}

void    OpenCL::_createCommandQueue( void ) {
    int     err;

    this->_commandQueue = clCreateCommandQueue(this->_context, this->_device, 0, &err);
    checkCLSuccess(err, "clCreateCommandQueue");
}

void    OpenCL::displayInformation( void ) {
    Platform    platform;

    platform.displayInfoPlatforms();
}

OpenCL::~OpenCL( void ) {
}
