// basic program for the ARDOS phantom. w. birkfellner, cmpbme, mu vienna, 2017

#include "qanalogphidgets.h"
#include <unistd.h>
#include <qdebug.h>

// reads up to 8 digital inputs via a dedicated board

//-----------------------------------------------------------------------------

QAnalogPhidgets::QAnalogPhidgets(void){
    int sernum, version, numOutputs, numInputs, numSensors;

    this->IFK = NULL;
    this->errorCreate = CPhidgetInterfaceKit_create(&IFK); // initialize the interface board
    this->errorOpen = CPhidget_open((CPhidgetHandle)IFK, -1);
    sleep(3);
    CPhidget_getSerialNumber((CPhidgetHandle)IFK, &sernum);
    CPhidget_getDeviceVersion((CPhidgetHandle)IFK, &version);
    CPhidgetInterfaceKit_getOutputCount((CPhidgetInterfaceKitHandle)IFK, &numOutputs);
    CPhidgetInterfaceKit_getInputCount((CPhidgetInterfaceKitHandle)IFK, &numInputs);
    CPhidgetInterfaceKit_getSensorCount((CPhidgetInterfaceKitHandle)IFK, &numSensors);
    this->snumifk=sernum;
    this->vifk=version;
    this->nOutifk=numOutputs;
    this->nInifk=numInputs;
    this->nSensorsifk=numSensors; // jsut get basic data on the board
}

//-----------------------------------------------------------------------------

QAnalogPhidgets::~QAnalogPhidgets(void){
    CPhidget_close((CPhidgetHandle)IFK);
    CPhidget_delete((CPhidgetHandle)IFK);
    sleep(1);
}

//-----------------------------------------------------------------------------

int QAnalogPhidgets::retrievePhidgetSensordata(int port) { // read the eight digital inputs. analog inputs are not used
    int val=0;
    float preciseval=0;

    this->errorRead = CPhidgetInterfaceKit_getSensorValue(IFK, port, &val);
    if (this->errorRead == 0) {

    } else {
        this->topicalAnalogReading = 0;
    }
    this->topicalAnalogReading=(int)preciseval;
    return this->topicalAnalogReading;
}

//-----------------------------------------------------------------------------

int QAnalogPhidgets::retrievePhidgetInterfaceStatus(void) { // pf ... does basically nothing. error handler
    if ((this->errorCreate == 0) && (this->errorOpen == 0)) {
        return 0;
    } else {
        return 1;
    }
}

//-----------------------------------------------------------------------------

int QAnalogPhidgets::retrievePhidgetControllerData(int what) {
    int retval;

    switch (what) {
    case 1:
        retval=this->snumifk;
        break;
    case 2:
        retval=this->vifk;
        break;
    case 3:
        retval=this->nOutifk;
        break;
    case 4:
        retval=this->nInifk;
        break;
    case 5:
        retval=this->nSensorsifk;
        break;
    default:
        retval=-1;
    }
    return retval;
}

//-----------------------------------------------------------------------------

void QAnalogPhidgets::setDigitalOnOrOff(int whichOne, int isOn) { // sets digital outputs. not used.
    if ((whichOne >= 0) && (whichOne < 8) && (isOn >=0) && (isOn <= 1)) {
           CPhidgetInterfaceKit_setOutputState((CPhidgetInterfaceKitHandle)IFK,whichOne, isOn);
    }
}

//-----------------------------------------------------------------------------

short QAnalogPhidgets::getDigitalInput(int whichOne) { // reads digital inputs
    int value, sval = 0;

    if ((whichOne >= 0) && (whichOne < 8))  {
           CPhidgetInterfaceKit_getInputState((CPhidgetInterfaceKitHandle)IFK,whichOne, &value);
           sval = (short)value;
    }
    return sval;
}

//-----------------------------------------------------------------------------
