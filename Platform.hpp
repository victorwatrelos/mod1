#ifndef PLATFORM_HPP
# define PLATFORM_HPP

# include <OpenCL/cl.h>
# include <string>
# include <iostream>
# include <vector>
# include "clUtil.hpp"
# include "Device.hpp"

class Platform {
    public:
        Platform( void );
        void    displayInfoPlatforms();
        cl_device_id    getDevice(int platformId, int deviceId);
    private:
        cl_uint             numPlatforms;
        cl_platform_id      *platformIds;
        void                queryInfoPlatform(cl_platform_info query, int idPlatform, std::string paramName);
        std::vector<Device *> _devices;
};

#endif
