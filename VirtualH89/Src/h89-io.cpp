/// \file h89-io.cpp
///
/// \date Mar 7, 2009
/// \author Mark Garlanger
///
/// \brief Implements framework to handle all the I/O devices on the H89.
///

#include "h89-io.h"

#include "logger.h"
#include "IODevice.h"
#include "DiskController.h"


H89_IO::H89_IO(): IOBus()
{
    debugss(ssIO, INFO, "%\n");

}

H89_IO::~H89_IO()
{
    debugss(ssIO, INFO, "\n");
}



std::vector<DiskController*>&
H89_IO::getDiskDevices()
{
    return dsk_devs;
}

bool
H89_IO::addDiskDevice(DiskController* device)
{
    dsk_devs.push_back(device);
    return addDevice(device);
}
