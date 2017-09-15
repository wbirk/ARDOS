// basic program for the ARDOS phantom. w. birkfellner, cmpbme, mu vienna, 2017

#include "qstepperphidgets.h"
#include <unistd.h>
#include <stdlib.h>
#include <qdebug.h>
#include <QElapsedTimer>
#include <math.h>

// stepper drive interface. 4 of these are instatniated in the program.

//-----------------------------------------------------------------------------

QStepperPhidgets::QStepperPhidgets(void){
    int sernum, version,stepc, what;
    double smax, amax, smin;

    this->SH = NULL;
    this->errorCreate = CPhidgetStepper_create(&SH);
    this->errorOpen = CPhidget_open((CPhidgetHandle)SH, -1); // initialize the board
    sleep(1); // give the board some time to react
    CPhidget_getSerialNumber((CPhidgetHandle)SH, &sernum);
    CPhidget_getDeviceVersion((CPhidgetHandle)SH, &version);
    sleep(1);  // give the board some time to react
    this->snumifk=sernum;
    this->vifk=version; // retrivieng basic data
    CPhidgetStepper_getVelocityMax((CPhidgetStepperHandle)SH,0,&smax);
    CPhidgetStepper_getVelocityMin((CPhidgetStepperHandle)SH,0,&smin);
    CPhidgetStepper_getAccelerationMax((CPhidgetStepperHandle)SH,0,&amax); // getting theoretical maximal values
    CPhidgetStepper_setCurrentLimit((CPhidgetStepperHandle)SH,0,2.5); // initialize the current to something. is overridden by the GUI
    this->speedMax=smax*0.4;
    this->speedMin=smin;
    this->accMax=amax/10;
    this->currSpeedMax=speedMax;
    this->currSpeedMin=speedMin;
    this->driveIsActive=false; // basic parameters
    for (stepc = 0; stepc < 8; stepc++ ) {
        for (what = 0; what < 3; what++) {
            this->lLoopTrajectory[stepc][what]=0;
            this->fLoopSet = false;
        }
    }
    this->gearRatio = 1;
    this->trajectoryIsDone = false;
}

//-----------------------------------------------------------------------------

QStepperPhidgets::~QStepperPhidgets(void){
    this->stopped=PTRUE;
    CPhidgetStepper_setCurrentPosition((CPhidgetStepperHandle)SH, 0, 0);
    CPhidgetStepper_setEngaged((CPhidgetStepperHandle)SH, 0, 0);
    sleep(1);
    CPhidget_close((CPhidgetHandle)SH);
    CPhidget_delete((CPhidgetHandle)SH);
}


//-----------------------------------------------------------------------------
void QStepperPhidgets::setLoop(short what, short stp, double val) {
    if ((what >= 0) && (what < 3)) {
        if ((stp >= 0) && (stp < 8)) {
            this->lLoopTrajectory[stp][what]=val;
        }
    }
    this->fLoopSet = true;
}

//-----------------------------------------------------------------------------
void QStepperPhidgets::deleteLoop(void) {
    short step, what;

    for (step = 0; step < 8; step++) {
        for (what = 0; what++; what++) {
            this->lLoopTrajectory[step][what]=0;
        }
    }
    this->fLoopSet = false;
}

//-----------------------------------------------------------------------------

int QStepperPhidgets::sendSteps(int noOfSteps) { // well - sends a number of microsteps to the controller and takes care of curent, acc and speed.
    // these parameters are provided by the GUI
    long long int stepsDone;
    int iSD = 0;

    CPhidgetStepper_setAcceleration((CPhidgetStepperHandle)SH,0,this->accMax); //currAccMax
    CPhidgetStepper_setVelocityLimit((CPhidgetStepperHandle)SH,0,this->currSpeedMax);
    CPhidgetStepper_setEngaged((CPhidgetStepperHandle)SH, 0, 1);
    CPhidgetStepper_setCurrentPosition((CPhidgetStepperHandle)SH, 0, 0);
    CPhidgetStepper_setTargetPosition((CPhidgetStepperHandle)SH, 0, noOfSteps);

    this->stopped = false;
    while (!stopped) {
        CPhidgetStepper_getStopped((CPhidgetStepperHandle)SH, 0, &stopped);
    }
    CPhidgetStepper_getCurrentPosition((CPhidgetStepperHandle)SH, 0, &stepsDone);
    CPhidgetStepper_setEngaged((CPhidgetStepperHandle)SH, 0, 0);
    iSD=(int)stepsDone;
    return iSD;
}

//-----------------------------------------------------------------------------
void QStepperPhidgets::loopDriveComplex(void) {
    QElapsedTimer *restphase;
    short trajCount;

    if ((fLoopSet == true) && (this->trajectoryIsDone == false)) {
        CPhidgetStepper_setEngaged((CPhidgetStepperHandle)SH, 0, 1);
        CPhidgetStepper_setAcceleration((CPhidgetStepperHandle)SH,0,this->accMax);
        for (trajCount = 0; trajCount < 8; trajCount++) {
            CPhidgetStepper_setVelocityLimit((CPhidgetStepperHandle)SH,0,round(this->lLoopTrajectory[trajCount][1]*this->gearRatio)); // set speed
            CPhidgetStepper_setCurrentPosition((CPhidgetStepperHandle)SH, 0, 0);
            CPhidgetStepper_setTargetPosition((CPhidgetStepperHandle)SH, 0, round(this->lLoopTrajectory[trajCount][0]*this->gearRatio)); // setSteps
            this->stopped = false;
            while (!stopped) {
                CPhidgetStepper_getStopped((CPhidgetStepperHandle)SH, 0, &stopped);
            }
            restphase = new QElapsedTimer();
            restphase->start();
            while (restphase->elapsed() < this->lLoopTrajectory[trajCount][2]) { }
            delete restphase;
        }
        this->trajectoryIsDone = true; // this flag is set when one ride is done.
    }
}

//-----------------------------------------------------------------------------
void QStepperPhidgets::setTrajectoryDoneFlag(bool val) { // this method sets the "trajectoryIsDone" flag
    this->trajectoryIsDone = val;
}

//-----------------------------------------------------------------------------
bool QStepperPhidgets::getTrajectoryDoneFlag(void) { // retrieves the value of the flag
    return this->trajectoryIsDone;
}

//-----------------------------------------------------------------------------
int QStepperPhidgets::retrievePhidgetStepperData(short what) { // get basic parameters from the class
    int retval;

    switch (what) {
    case 1:
        retval=this->snumifk;
        break;
    case 2:
        retval=this->vifk;
        break;
    case 3:
        retval=this->motorNum;
        break;
    default:
        retval=-1;
    }
    return retval;
}

//-----------------------------------------------------------------------------

double QStepperPhidgets::getKinetics(short whichOne) { // get settings for the stepper from the class
    double retval;

    switch (whichOne) {
    case 1:
        retval = (this->accMax); // setting acceleration is disabled
        break;
    case 2:
        retval = (this->accMax);
        break;
    case 3:
        retval = (this->currSpeedMax);
        break;
    case 4:
        retval = (this->currSpeedMin);
        break;
    default:
        retval=-1;
    }
    return retval;
}

//-----------------------------------------------------------------------------


void QStepperPhidgets::setKinetics(double val, short whichOne) { // set parameters to the steppers

    switch (whichOne) {
    case 1:
//        this->currAccMax=val; setting acceleration is disabled
        break;
    case 2:
        this->currSpeedMax=val;
        break;
    case 3:
        this->currentMax=val;
        CPhidgetStepper_setCurrentLimit((CPhidgetStepperHandle)SH,0,this->currentMax);
    }
    return;
}

//-----------------------------------------------------------------------------

void QStepperPhidgets::shutDownDrive(void) { // halt the drive

    CPhidgetStepper_setEngaged((CPhidgetStepperHandle)SH, 0, 0);
    CPhidgetStepper_setCurrentPosition((CPhidgetStepperHandle)SH, 0, 0);
    CPhidgetStepper_setTargetPosition((CPhidgetStepperHandle)SH, 0, 0);
    this->stopped=PTRUE;
}

//-----------------------------------------------------------------------------

void QStepperPhidgets::setDriveActive(bool toWhat) { // set an internal flag
    this->driveIsActive = toWhat;
}

//-----------------------------------------------------------------------------

bool QStepperPhidgets::getDriveActive(void) {  // retrieve the flag
    return this->driveIsActive;
}

//-----------------------------------------------------------------------------

void QStepperPhidgets::setLoopFlag(bool toWhat) { // set a flag whether a trajectory was defined ...
    this->fLoopSet = toWhat;
}

//-----------------------------------------------------------------------------

bool QStepperPhidgets::getLoopFlag(void) {
    return this->fLoopSet;
}

//-----------------------------------------------------------------------------
void QStepperPhidgets::setGearRatio(double value) {
    this->gearRatio = value;
}
