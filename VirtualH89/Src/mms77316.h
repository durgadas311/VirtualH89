/// \file mms77316.h
///
/// Implementation of the MMS 77316 floppy disk controller.
///
/// \date Jan 30, 2016
/// \author Douglas Miller, cloned from h37.h by Mark Garlanger
///

#ifndef MMS77316_H_
#define MMS77316_H_


#include "wd1797.h"
#include "DiskController.h"
#include "propertyutil.h"

class GenericFloppyDrive;
class GenericDiskDrive;
class InterruptController;

///
/// \brief Virtual soft-sectored disk controller
///
/// A virtual Magnolia Microsystems soft-sectored disk controller (MMS77316).
///
/// The MMS77316 uses the 1797-02 controller.
///
class MMS77316: public DiskController, WD1797
{
  public:
    static const int numDisks_c = 8;

    MMS77316(int baseAddr, InterruptController* ic);
    virtual ~MMS77316();

    static MMS77316* install_MMS77316(InterruptController*        ic,
                                      PropertyUtil::PropertyMapT& props,
                                      std::string                 slot);

    virtual BYTE in(BYTE addr);
    virtual void out(BYTE addr,
                     BYTE val);

    virtual bool connectDrive(BYTE                unitNum,
                              GenericFloppyDrive* drive);
    virtual bool removeDrive(BYTE unitNum);
    virtual GenericFloppyDrive* getDrive(BYTE unitNum);

    virtual void reset(void);

    static const BYTE MMS77316_Intr_c = 5; // INT 5

    // TODO: implement this
    std::vector<GenericDiskDrive*> getDiskDrives();
    std::string getDriveName(int index);
    std::string getDeviceName()
    {
        return MMS77316_Name_c;
    }
    GenericDiskDrive* findDrive(std::string ident);
    std::string dumpDebug();

  protected:
    static const char* MMS77316_Name_c;

  private:
    void raiseIntrq();
    void raiseDrq();
    void lowerIntrq();
    void lowerDrq();

    static const BYTE MMS77316_NumPorts_c  = 8;

    static const BYTE BasePort_c           = 0x38;
    static const BYTE ControlPort_Offset_c = 0;
    static const BYTE Wd1797_Offset_c      = 4;
    BYTE              controlReg_m;

    GenericFloppyDrive* getCurDrive();
    int getClockPeriod();

    InterruptController* ic_m;
    GenericFloppyDrive*  drives_m[numDisks_c];

    unsigned char        intLevel_m;
    int                  drqCount_m;

    /// Bits set in cmd_ControlPort_c
    static const BYTE    ctrl_EnableIntReq_c     = 0x08;
    static const BYTE    ctrl_EnableBurstN_c     = 0x20;
    static const BYTE    ctrl_SetMFMRecordingN_c = 0x40;
    static const BYTE    ctrl_DriveSel_c         = 0x07;
    static const BYTE    ctrl_525DriveSel_c      = 0x04;

    bool burstMode()
    {
        return (controlReg_m & ctrl_EnableBurstN_c) == 0;
    }
    bool intrqAllowed()
    {
        return (controlReg_m & ctrl_EnableIntReq_c) != 0;
    }
    bool drqAllowed()
    {
        return (controlReg_m & ctrl_EnableIntReq_c) != 0 &&
               ((controlReg_m & ctrl_EnableBurstN_c) != 0 || drqCount_m < 1);
    }
    // These are virtual in wd1797 so it can use it.
    bool doubleDensity()
    {
        return (controlReg_m & ctrl_SetMFMRecordingN_c) == 0;
    }
    void loadHead(bool load);
};

#endif // MMS77316_H_
