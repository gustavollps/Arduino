#define  CTRL_REG1  0x20
#define  CTRL_REG2  0x21
#define  CTRL_REG3  0x22
#define  CTRL_REG4  0x23
#define  CTRL_REG5  0x24
#define  CTRL_REG6  0x25

#define ACC_X_OFFSET  -164.4
#define ACC_Y_OFFSET  -35.8
#define ACC_Z_OFFSET  590.0

#define ACC_X_GAIN  1.0
#define ACC_Y_GAIN  1.0
#define ACC_Z_GAIN  1.18

#define  NUM_GYRO_SAMPLES  50           // As recommended in STMicro doc
#define  GYRO_SIGMA_MULTIPLE  0         // As recommended 

float gyroRaw[3];                         // raw sensor data, each axis, pretty useless really but here it is.
double gyroDPS[3];                      // gyro degrees per second, each axis

float heading[3] = {0.0f};              // heading[x], heading[y], heading [z]
int headingint[3];

float gyroZeroRate[3];                    // Calibration data.  Needed because the sensor does center at zero, but rather always reports a small amount of rotation on each axis.
float gyroThreshold[3];                   // Raw rate change data less than the statistically derived threshold is discarded.

float dpsPerDigit = .00875f;            // for conversion to degrees per second
float angles[3];

int gyroI2CAddr = 105;
