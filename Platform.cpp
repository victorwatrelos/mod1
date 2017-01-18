#include "Platform.hpp"

Platform::Platform( void )
{
    cl_int      errNum;

    errNum = clGetPlatformIDs(0, NULL, &(this->numPlatforms));
    checkCLSuccess(errNum, "clGetPlatformIDs");
    this->platformIds = (cl_platform_id *)malloc(sizeof(cl_platform_id) * this->numPlatforms);
    errNum = clGetPlatformIDs(this->numPlatforms, this->platformIds, NULL);
    checkCLSuccess(errNum, "clGetPlatformIDs");
    for (cl_uint i = 0; i < this->numPlatforms; i++) {
        this->_devices.push_back(new Device(this->platformIds[i]));
    }
}

cl_device_id    Platform::getDevice(int platformId, int deviceId) {
    return (this->_devices[platformId]->getDeviceId(deviceId));
}

void    Platform::displayInfoPlatforms() {
    std::cout << "Nb platforms: " << this->numPlatforms << std::endl;
    for (cl_uint i = 0; i < this->numPlatforms; i++) {
        this->queryInfoPlatform(CL_PLATFORM_PROFILE, i, "PROFILE");
        this->queryInfoPlatform(CL_PLATFORM_VERSION, i, "VERSION");
        this->queryInfoPlatform(CL_PLATFORM_NAME, i, "NAME");
        this->queryInfoPlatform(CL_PLATFORM_VENDOR, i, "VENDOR");
        this->_devices[i]->displayInfoDevices();
    }
}

void    Platform::queryInfoPlatform(cl_platform_info query, int idPlatform, std::string paramName) {
    size_t  size;
    char    *response;

    checkCLSuccess(clGetPlatformInfo(this->platformIds[idPlatform], query, 0, NULL, &size),
            "clGetPlatformInfo");
    response = (char *)malloc(sizeof(char) * size);
    checkCLSuccess(clGetPlatformInfo(this->platformIds[idPlatform], query, size, response, NULL),
            "clGetPlatformInfo");
    std::cout << paramName << " : " << response << std::endl;
    free(response);
}

