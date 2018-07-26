#ifndef QSTEPPERPHIDGETS_H
#define QSTEPPERPHIDGETS_H

#include <phidget21.h>

class QStepperPhidgets {
private:
    CPhidgetStepperHandle SH;
    int errorOpen;
    int errorCreate;
    int snumifk;
    int vifk;
    int motorNum;
    double speedMax;
    double speedMin;
    double accMax;
    double currSpeedMax;
    double currSpeedMin;
    double currentMax;
    int stopped;
    bool driveIsActive;
    double lLoopTrajectory[8][3];
    bool fLoopSet;
    double gearRatio;
    bool trajectoryIsDone;

public:
    QStepperPhidgets(void);
    ~QStepperPhidgets(void);
    int sendSteps(int);
    int retrievePhidgetStepperData (short);
    double getKinetics(short);
    void setKinetics(double, short);
    void shutDownDrive(void);
    void loopDriveComplex(void);
    void setLoop(short,short,double);
    void deleteLoop(void);
    void setDriveActive(bool);
    bool getDriveActive(void);
    void setLoopFlag(bool);
    bool getLoopFlag(void);
    void setGearRatio(double);
    void setTrajectoryDoneFlag(bool);
    bool getTrajectoryDoneFlag(void);
};
#endif // QSTEPPERPHIDGETS_H
