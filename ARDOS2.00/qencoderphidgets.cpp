// basic program for the ARDOS phantom. w. birkfellner, cmpbme, mu vienna, 2017

#include "qencoderphidgets.h"
#include <unistd.h>
#include <stdlib.h>
#include <qdebug.h>

// encoder interface. not used yet ...

//-----------------------------------------------------------------------------

QEncoderPhidgets::QEncoderPhidgets(void){
    int sernum, version, numEncs, numInps;

    this->EH = NULL;
    this->errorCreate = CPhidgetEncoder_create(&EH);
    this->errorOpen = CPhidget_open((CPhidgetHandle)EH, -1);
    sleep(1);
    CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)EH,0,1);
    CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)EH,1,1);
    CPhidget_getSerialNumber((CPhidgetHandle)EH, &sernum);
    CPhidget_getDeviceVersion((CPhidgetHandle)EH, &version);
    CPhidgetEncoder_getEncoderCount (EH, &numEncs);
    CPhidgetEncoder_getInputCount (EH, &numInps);
    this->snumifk=sernum;
    this->vifk=version;
    this->EncoderNum=numEncs;
    this->InputNum=numInps;
    this->DirCalFactor=1;
    this->LinCalFactor=1;
}

//-----------------------------------------------------------------------------

QEncoderPhidgets::~QEncoderPhidgets(void){
    CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)EH,0,0);
    CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)EH,1,0);
    CPhidget_close((CPhidgetHandle)EH);
    CPhidget_delete((CPhidgetHandle)EH);
    sleep(1);
}

//-----------------------------------------------------------------------------

int QEncoderPhidgets::getTopicalReadingFromEncoder(int which) {
    int stepsDone;

    CPhidgetEncoder_getPosition(EH, which, &stepsDone);
    return stepsDone;
}

//-----------------------------------------------------------------------------

void QEncoderPhidgets::resetEncoder(int which) {
    CPhidgetEncoder_setPosition(EH, which, 0);
}

//-----------------------------------------------------------------------------

int QEncoderPhidgets::retrievePhidgetEncoderData(int what) {
    int retval;

    switch (what) {
    case 1:
        retval=this->snumifk;
        break;
    case 2:
        retval=this->vifk;
        break;
    case 3:
        retval=this->EncoderNum;
        break;
    case 4:
        retval=this->InputNum;
        break;
    default:
        retval=-1;
    }
    return retval;
}

//-----------------------------------------------------------------------------


void QEncoderPhidgets::setCalibrationFactor(int which, float cf) {
    if (which == 0) {
        this->DirCalFactor = cf;
    } else {
        this->LinCalFactor = cf;
    }
}

//-----------------------------------------------------------------------------

float QEncoderPhidgets::getCalibrationFactor(int which) {
       if (which == 0) {
           return this->DirCalFactor;
       } else {
           return this->LinCalFactor;
       }
}

