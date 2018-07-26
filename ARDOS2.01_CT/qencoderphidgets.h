#ifndef QENCODERPHIDGETS_H
#define QENCODERPHIDGETS_H

#include <phidget21.h>

class QEncoderPhidgets {
private:
    CPhidgetEncoderHandle EH;
    int errorOpen;
    int errorCreate;
    int snumifk;
    int vifk;
    int EncoderNum;
    int InputNum;
    float DirCalFactor;
    float LinCalFactor;

public:
    QEncoderPhidgets(void);
    ~QEncoderPhidgets(void);
    int getTopicalReadingFromEncoder(int);
    int retrievePhidgetEncoderData (int);
    void resetEncoder(int);
    float getCalibrationFactor(int);
    void setCalibrationFactor(int, float);
};

#endif // QENCODERPHIDGETS_H
