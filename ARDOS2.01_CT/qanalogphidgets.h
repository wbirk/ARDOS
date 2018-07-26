#ifndef QANALOGPHIDGETS_H
#define QANALOGPHIDGETS_H

#include <phidget21.h>

class QAnalogPhidgets {
private:
    CPhidgetInterfaceKitHandle IFK;
    int errorOpen;
    int errorCreate;
    int errorRead;
    int topicalAnalogReading;
    int snumifk;
    int vifk;
    int nOutifk;
    int nInifk;
    int nSensorsifk;
    int openPhidgetInterface(void);
    int closePhidgetInterface(void);
public:
    QAnalogPhidgets(void);
    ~QAnalogPhidgets(void);
    int retrievePhidgetSensordata(int port);
    int retrievePhidgetInterfaceStatus(void);
    int retrievePhidgetControllerData(int typeOfData);
    void setDigitalOnOrOff(int, int);
    short getDigitalInput(int);
};



#endif // QANALOGPHIDGETS_H
