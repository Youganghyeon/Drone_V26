/*
  This is a library written for the BNO080
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14686

  Written by Nathan Seidle @ SparkFun Electronics, December 28th, 2017

  The BNO080 IMU is a powerful triple axis gyro/accel/magnetometer coupled with an ARM processor
  to maintain and complete all the complex calculations for various VR, inertial, step counting,
  and movement operations.

  This library handles the initialization of the BNO080 and is able to query the sensor
  for different readings.

  https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.5

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * This library source code has been modified for STM32F4. Only supports SPI.
 *
 * Development environment specifics:
 * STM32CubeIDE 1.0.0
 * STM32CubeF4 FW V1.24.1
 * STM32F4 LL Driver(SPI) and HAL Driver(RCC for HAL_Delay() function)
 *
 * Modified by ChrisP(Wonyeob Park) @ M-HIVE Embedded Academy, June, 2019
 * Rev. 1.0
 *
 * https://github.com/ChrisWonyeobPark/BNO080-STM32F4-SPI-LL-Driver
 * https://www.udemy.com/course/stm32-drone-programming/?referralCode=E24CB7B1CD9993855D45
 * https://www.inflearn.com/course/stm32cubelde-stm32f4%EB%93%9C%EB%A1%A0-%EA%B0%9C%EB%B0%9C
 */

#ifndef	_BNO080_H
#define	_BNO080_H

#include "hw_def.h"
//////////////////////////////////////////////////////////////////////////
#ifdef _USE_HW_BNO080
/**
 * @brief Definition for connected to SPI2 (APB1 PCLK = 42MHz)
 *
 */

#define MAX_PACKET_SIZE 128 //Packets can be up to 32k but we don't have that much RAM.
#define MAX_METADATA_SIZE 9 //This is in words. There can be many but we mostly only care about the first 9 (Qs, range, etc)

typedef enum {
  rawAccel  = 0,
  rawLinAccel ,
  rawGyro,
  rawMag,
  rawQuat
}getMode_t;

typedef enum{
  QuatI = 0,
  QuatJ ,
  QuatK,
  QuatReal,
  QuatRadianAccuracy,
  AccelX,
  AccelY,
  AccelZ,
}getValue_t;

//
//#define  rawAccel     0
//#define  rawLinAccel  1
//#define  rawGyro      2
//#define  rawMag       3
//Global Variables
typedef struct{
  uint8_t shtpHeader[4]; //Each packet has a header of 4 bytes
  uint8_t shtpData[MAX_PACKET_SIZE];
  uint8_t sequenceNumber[6];
  uint8_t commandSequenceNumber;        //Commands have a seqNum as well. These are inside command packet, the header uses its own seqNum per channel
  uint32_t metaData[MAX_METADATA_SIZE];     //There is more than 10 words in a metadata record but we'll stop at Q point 3
}BNO080_Packet_tbl;

typedef struct{
  uint16_t rawX;
  uint16_t rawY;
  uint16_t rawZ;
  uint16_t accuracy;
}BNO080_Raw_tbl;

typedef struct{
  uint16_t rawQuatI;
  uint16_t rawQuatJ;
  uint16_t rawQuatK;
  uint16_t rawQuatReal;
  uint16_t rawQuatRadianAccuracy;
  uint16_t quatAccuracy;
}BNO080_Quaternion_tbl;

typedef struct{
  float Roll;
  float Pitch;
  float Yaw;
}BNO080_Angle_tbl;

typedef struct{
  BNO080_Raw_tbl        BNO080_Raw[4];
  BNO080_Packet_tbl     BNO080_Packet;
  BNO080_Quaternion_tbl BNO080_Quat;
  BNO080_Angle_tbl      BNO080_Angle;
  uint16_t              stepCount;
  uint32_t              timeStamp;
  uint8_t               stabilityClassifier;
  uint8_t               activityClassifier;
  uint8_t               activityConfidences[9]; //Array that store the confidences of the 9 possible activities
  uint8_t               calibrationStatus;   //Byte R0 of ME Calibration Response
}BNO080_tbl;



#if 0
#define BNO080_SPI_CHANNEL		SPI2

#define BNO080_SPI_SCLK_PIN		LL_GPIO_PIN_13
#define BNO080_SPI_SCLK_PORT	GPIOB
#define BNO080_SPI_SCLK_CLK		LL_AHB1_GRP1_PERIPH_GPIOB

#define BNO080_SPI_MISO_PIN		LL_GPIO_PIN_14
#define BNO080_SPI_MISO_PORT	GPIOB
#define BNO080_SPI_MISO_CLK		LL_AHB1_GRP1_PERIPH_GPIOB

#define BNO080_SPI_MOSI_PIN		LL_GPIO_PIN_15
#define BNO080_SPI_MOSI_PORT	GPIOB
#define BNO080_SPI_MOSI_CLK		LL_AHB1_GRP1_PERIPH_GPIOB
#endif
#define BNO080_SPI_CS_PIN		GPIO_PIN_12
#define BNO080_SPI_CS_PORT		GPIOB
#define BNO080_SPI_CS_CLK		AHB1_GRP1_PERIPH_GPIOB

#define BNO080_PS0_WAKE_PIN		GPIO_PIN_8
#define BNO080_PS0_WAKE_PORT	GPIOA
#define BNO080_PS0_WAKE_CLK		AHB1_GRP1_PERIPH_GPIOA

#define BNO080_RST_PIN			GPIO_PIN_9
#define BNO080_RST_PORT			GPIOC
#define BNO080_RST_CLK			AHB1_GRP1_PERIPH_GPIOC

#define BNO080_INT_PIN			GPIO_PIN_8
#define BNO080_INT_PORT			GPIOC
#define BNO080_INT_CLK			AHB1_GRP1_PERIPH_GPIOC

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define WAKE_HIGH()				HAL_GPIO_WritePin(BNO080_PS0_WAKE_PORT, BNO080_PS0_WAKE_PIN,SET)
#define WAKE_LOW()				HAL_GPIO_WritePin(BNO080_PS0_WAKE_PORT, BNO080_PS0_WAKE_PIN,RESET)

#define RESET_HIGH()			HAL_GPIO_WritePin(BNO080_RST_PORT, BNO080_RST_PIN,SET)
#define RESET_LOW()				HAL_GPIO_WritePin(BNO080_RST_PORT, BNO080_RST_PIN,RESET)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Registers
enum Registers
{
	CHANNEL_COMMAND = 0,
	CHANNEL_EXECUTABLE = 1,
	CHANNEL_CONTROL = 2,
	CHANNEL_REPORTS = 3,
	CHANNEL_WAKE_REPORTS = 4,
	CHANNEL_GYRO = 5
};

//All the ways we can configure or talk to the BNO080, figure 34, page 36 reference manual
//These are used for low level communication with the sensor, on channel 2
#define SHTP_REPORT_COMMAND_RESPONSE 0xF1
#define SHTP_REPORT_COMMAND_REQUEST 0xF2
#define SHTP_REPORT_FRS_READ_RESPONSE 0xF3
#define SHTP_REPORT_FRS_READ_REQUEST 0xF4
#define SHTP_REPORT_PRODUCT_ID_RESPONSE 0xF8
#define SHTP_REPORT_PRODUCT_ID_REQUEST 0xF9
#define SHTP_REPORT_BASE_TIMESTAMP 0xFB
#define SHTP_REPORT_SET_FEATURE_COMMAND 0xFD

//All the different sensors and features we can get reports from
//These are used when enabling a given sensor
typedef enum{
  SENSOR_REPORTID_ACCELEROMETER                 = 0x01,
  SENSOR_REPORTID_GYROSCOPE                     = 0x02,
  SENSOR_REPORTID_MAGNETIC_FIELD                = 0x03,
  SENSOR_REPORTID_LINEAR_ACCELERATION           = 0x04,
  SENSOR_REPORTID_ROTATION_VECTOR               = 0x05,
  SENSOR_REPORTID_GRAVITY                       = 0x06,
  SENSOR_REPORTID_GAME_ROTATION_VECTOR          = 0x08,
  SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR   = 0x09,
  SENSOR_REPORTID_TAP_DETECTOR                  = 0x10,
  SENSOR_REPORTID_STEP_COUNTER                  = 0x11,
  SENSOR_REPORTID_STABILITY_CLASSIFIER          = 0x13,
  SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER  = 0x1E
}Sensor_Mode_t;

//Record IDs from figure 29, page 29 reference manual
//These are used to read the metadata for each sensor type
#define FRS_RECORDID_ACCELEROMETER 0xE302
#define FRS_RECORDID_GYROSCOPE_CALIBRATED 0xE306
#define FRS_RECORDID_MAGNETIC_FIELD_CALIBRATED 0xE309
#define FRS_RECORDID_ROTATION_VECTOR 0xE30B

//Command IDs from section 6.4, page 42
//These are used to calibrate, initialize, set orientation, tare etc the sensor

#define COMMAND_ERRORS 1
#define COMMAND_COUNTER 2
#define COMMAND_TARE 3
#define COMMAND_INITIALIZE 4
#define COMMAND_DCD 6
#define COMMAND_ME_CALIBRATE 7
#define COMMAND_DCD_PERIOD_SAVE 9
#define COMMAND_OSCILLATOR 10
#define COMMAND_CLEAR_DCD 11

typedef enum{
 CALIBRATE_ACCEL= 0x00,
 CALIBRATE_GYRO,
 CALIBRATE_MAG,
 CALIBRATE_PLANAR_ACCEL,
 CALIBRATE_ACCEL_GYRO_MAG,
 CALIBRATE_STOP,
 CALIBRATE_MAX
}Calibrate_Mode_t;


void BNO080_GPIO_SPI_Initialization(void);
bool BNO080_Init(void);
bool BNO080_Open(BNO080_tbl* p_sensor);
void BNO080_parseCommandReport(BNO080_tbl* p_sensor);
void BNO080_parseInputReport(BNO080_tbl* p_sensor);
//void BNO080_enableRotationVector(uint16_t timeBetweenReports);
void BNO080_enableGameRotationVector(BNO080_tbl* p_sensor, uint16_t timeBetweenReports);
void BNO080_calibrateAll();
void BNO080_enableMagnetometer(BNO080_tbl* p_sensor, uint16_t timeBetweenReports);
void BNO080_saveCalibration();
void BNO080_requestCalibrationStatus();
bool BNO080_calibrationComplete(BNO080_tbl* p_sensor);
bool BNO080_waitForSPI();
bool BNO080_receivePacket(BNO080_Packet_tbl* p_packet);
bool BNO080_sendPacket(BNO080_Packet_tbl* p_packet, uint8_t channelNumber, uint8_t dataLength);
uint8_t BNO080_getQuatAccuracy();
uint8_t BNO080_getMagAccuracy();
float BNO080_getValue(BNO080_tbl* p_sensor, getMode_t mode, getValue_t value);
bool BNO080_ReadInfo(BNO080_tbl* p_sensor);
bool BNO080_Update(float* q, BNO080_tbl* p_sensor);
void BNO080_enableRotationVector(BNO080_tbl* p_sensor, uint16_t timeBetweenReports);
bool BNO080_Calibration(BNO080_tbl* p_bno080, uint8_t isCaliOk);
#if 0

float BNO080_getAccelX();
float BNO080_getAccelY();
float BNO080_getAccelZ();
uint8_t BNO080_getAccelAccuracy();
float BNO080_getLinAccelX();
float BNO080_getLinAccelY();
float BNO080_getLinAccelZ();
uint8_t BNO080_getLinAccelAccuracy();
float BNO080_getGyroX();
float BNO080_getGyroY();
float BNO080_getGyroZ();
uint8_t BNO080_getGyroAccuracy();
float BNO080_getMagX();
float BNO080_getMagY();
float BNO080_getMagZ();
uint8_t BNO080_getMagAccuracy();
uint16_t BNO080_getStepCount();
uint8_t BNO080_getStabilityClassifier();
uint8_t BNO080_getActivityClassifier();
uint32_t BNO080_getTimeStamp();
int16_t BNO080_getQ1(uint16_t recordID);
int16_t BNO080_getQ2(uint16_t recordID);
int16_t BNO080_getQ3(uint16_t recordID);
float BNO080_getResolution(uint16_t recordID);
float BNO080_getRange(uint16_t recordID);

uint32_t BNO080_readFRSword(uint16_t recordID, uint8_t wordNumber);
void BNO080_frsReadRequest(uint16_t recordID, uint16_t readOffset, uint16_t blockSize);
int BNO080_readFRSdata(uint16_t recordID, uint8_t startLocation, uint8_t wordsToRead);
void BNO080_softReset(void);
uint8_t BNO080_resetReason();

float BNO080_qToFloat(int16_t fixedPointValue, uint8_t qPoint);
void BNO080_enableAccelerometer(uint16_t timeBetweenReports);
void BNO080_enableLinearAccelerometer(uint16_t timeBetweenReports);
void BNO080_enableGyro(uint16_t timeBetweenReports);
void BNO080_enableStepCounter(uint16_t timeBetweenReports);
void BNO080_enableStabilityClassifier(uint16_t timeBetweenReports);

void BNO080_calibrateAccelerometer();
void BNO080_calibrateGyro();
void BNO080_calibrateMagnetometer();
void BNO080_calibratePlanarAccelerometer();
void BNO080_calibrateAll();
void BNO080_endCalibration();

void BNO080_setFeatureCommand(BNO080_Packet_tbl* p_packet, uint8_t reportID, uint32_t microsBetweenReports, uint32_t specificConfig);
void BNO080_sendCommand(BNO080_Packet_tbl* p_packet, uint8_t command);
void BNO080_sendCalibrateCommand(BNO080_tbl* p_sensor, uint8_t thingToCalibrate);
void BNO080_saveCalibration();
#endif
#endif
#endif
