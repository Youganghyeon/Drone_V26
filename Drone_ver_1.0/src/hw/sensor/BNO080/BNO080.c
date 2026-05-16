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

#include "BNO080.h"
#include <math.h>

#define  DEF_BNO080  DEF_HW_BNO080
#define  rawAccel     0
#define  rawLinAccel  1
#define  rawGyro      2
#define  rawMag       3
//Global Variables
typedef struct{
  uint8_t shtpHeader[4]; //Each packet has a header of 4 bytes
  uint8_t shtpData[MAX_PACKET_SIZE];
  uint8_t sequenceNumber[6] = {0, 0, 0, 0, 0, 0}; //There are 6 com channels. Each channel has its own seqnum
  uint8_t commandSequenceNumber = 0;        //Commands have a seqNum as well. These are inside command packet, the header uses its own seqNum per channel
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
  BNO080_Raw_tbl BNO080_Raw[4];
  BNO080_Packet_tbl BNO080_Packet;
  BNO080_Quaternion_tbl BNO080_Quat;
  uint16_t stepCount;
  uint32_t timeStamp;
  uint8_t stabilityClassifier;
  uint8_t activityClassifier;
  uint8_t *_activityConfidences; //Array that store the confidences of the 9 possible activities
  uint8_t calibrationStatus;   //Byte R0 of ME Calibration Response
}BNO080_tbl;

BNO080_tbl BNO080;

//These Q values are defined in the datasheet but can also be obtained by querying the meta data records
//See the read metadata example for more info
static int16_t rotationVector_Q1 = 14;
static int16_t accelerometer_Q1 = 8;
static int16_t linear_accelerometer_Q1 = 8;
static int16_t gyro_Q1 = 9;
static int16_t magnetometer_Q1 = 4;


void BNO080_GPIO_SPI_Initialization(void)
{
	CHIP_DESELECT(BNO080);
	WAKE_HIGH();
	RESET_HIGH();
}

bool bno080Init(void)
{
  bool ret = false;
	BNO080_GPIO_SPI_Initialization();
	
	printf("Checking BNO080...");
	
	CHIP_DESELECT(BNO080);                        //CS HIGH
	                                             //Configure the BNO080 for SPI communication
	WAKE_HIGH();                              	//Before boot up the PS0/WAK pin must be high to enter SPI mode
	RESET_LOW();	                             //Reset BNO080
	delay(200);	                              //Min length not specified in datasheet?
	RESET_HIGH();	                           //Bring out of reset
	
	BNO080_waitForSPI(); //Wait until INT pin goes low.
	
	BNO080_waitForSPI(); //Wait for assertion of INT before reading advert message.
	BNO080_receivePacket();
	
	BNO080_waitForSPI();  //Wait for assertion of INT before reading Init response
	BNO080_receivePacket();
	
	//Check communication with device
	shtpData[0] = SHTP_REPORT_PRODUCT_ID_REQUEST; //Request the product ID and reset info
	shtpData[1] = 0;						 //Reserved
	
	BNO080_sendPacket(CHANNEL_CONTROL, 2);
	
	BNO080_waitForSPI();
	if(BNO080_receivePacket() == 1)
	{
   	if(shtpData[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE)
		{
		  ret=true; // Sensor OK
			//printf("BNO080 who_am_i = 0x%02x...ok\n\n", shtpData[0]);
		}
	}
	//printf("BNO080 Not OK: 0x%02x Should be 0x%02x\n", shtpData[0], SHTP_REPORT_PRODUCT_ID_RESPONSE);
	return ret; //Something went wrong
}

bool BNO080_TxRxByte(uint8_t *tx_data, uint8_t* rx_data)
{
  return SPI_PollByte(DEF_BNO080, tx_data, rx_data, 1);
}

bool BNO080_RxByte(uint8_t *tx_data, uint8_t* rx_data)
{
 return SPI
}


bool BNO080_dataAvailable(BNO080_tbl_t* p_sensor)
{

  bool ret= false;
	if(HAL_GPIO_ReadPin(BNO080_INT_PORT, BNO080_INT_PIN) == GPIO_PIN_SET)
	{
	  ret=false;
	}
	if(BNO080_receivePacket() == true)
	{
		if(shtpHeader[2] == CHANNEL_REPORTS && shtpData[0] == SHTP_REPORT_BASE_TIMESTAMP)
		{
			BNO080_parseInputReport();
			ret=true;
		}
		else if (shtpHeader[2] == CHANNEL_CONTROL)
		{
			BNO080_parseCommandReport();
			ret=true;
		}
	}
	return ret;
}

//This function pulls the data from the command response report

//Unit responds with packet that contains the following:
//shtpHeader[0:3]: First, a 4 byte header
//shtpData[0]: The Report ID
//shtpData[1]: Sequence number (See 6.5.18.2)
//shtpData[2]: Command
//shtpData[3]: Command Sequence Number
//shtpData[4]: Response Sequence Number
//shtpData[5 + 0]: R0
//shtpData[5 + 1]: R1
//shtpData[5 + 2]: R2
//shtpData[5 + 3]: R3
//shtpData[5 + 4]: R4
//shtpData[5 + 5]: R5
//shtpData[5 + 6]: R6
//shtpData[5 + 7]: R7
//shtpData[5 + 8]: R8
void BNO080_parseCommandReport(void)
{
	if (shtpData[0] == SHTP_REPORT_COMMAND_RESPONSE)
	{
		//The BNO080 responds with this report to command requests. It's up to use to remember which command we issued.
		uint8_t command = shtpData[2]; //This is the Command byte of the response

		if (command == COMMAND_ME_CALIBRATE)
		{
			calibrationStatus = shtpData[5]; //R0 - Status (0 = success, non-zero = fail)
		}
	}
	else
	{
		//This sensor report ID is unhandled.
		//See reference manual to add additional feature reports as needed
	}

	//TODO additional feature reports may be strung together. Parse them all.
}

//This function pulls the data from the input report
//The input reports vary in length so this function stores the various 16-bit values as globals

//Unit responds with packet that contains the following:
//shtpHeader[0:3]: First, a 4 byte header
//shtpData[0:4]: Then a 5 byte timestamp of microsecond clicks since reading was taken
//shtpData[5 + 0]: Then a feature report ID (0x01 for Accel, 0x05 for Rotation Vector)
//shtpData[5 + 1]: Sequence number (See 6.5.18.2)
//shtpData[5 + 2]: Status
//shtpData[3]: Delay
//shtpData[4:5]: i/accel x/gyro x/etc
//shtpData[6:7]: j/accel y/gyro y/etc
//shtpData[8:9]: k/accel z/gyro z/etc
//shtpData[10:11]: real/gyro temp/etc
//shtpData[12:13]: Accuracy estimate
void BNO080_parseInputReport(void)
{
	//Calculate the number of data bytes in this packet
	int16_t dataLength = ((uint16_t)shtpHeader[1] << 8 | shtpHeader[0]);
	dataLength &= ~(1 << 15); //Clear the MSbit. This bit indicates if this package is a continuation of the last.
	//Ignore it for now. TODO catch this as an error and exit

	dataLength -= 4; //Remove the header bytes from the data count

	timeStamp = ((uint32_t)shtpData[4] << (8 * 3)) | (shtpData[3] << (8 * 2)) | (shtpData[2] << (8 * 1)) | (shtpData[1] << (8 * 0));

	uint8_t status = shtpData[7] & 0x03; //Get status bits
	uint16_t data1 = (uint16_t)shtpData[10] << 8 | shtpData[9];
	uint16_t data2 = (uint16_t)shtpData[12] << 8 | shtpData[11];
	uint16_t data3 = (uint16_t)shtpData[14] << 8 | shtpData[13];
	uint16_t data4 = 0;
	uint16_t data5 = 0;

	if (dataLength > 14)
	{
		data4 = (uint16_t)shtpData[16] << 8 | shtpData[15];
	}
	if (dataLength > 16)
	{
		data5 = (uint16_t)shtpData[18] << 8 | shtpData[17];
	}

	//Store these generic values to their proper global variable
	switch(shtpData[5])
	{
		case SENSOR_REPORTID_ACCELEROMETER:
		{
			accelAccuracy = status;
			rawAccelX = data1;
			rawAccelY = data2;
			rawAccelZ = data3;
			break;
		}
		case SENSOR_REPORTID_LINEAR_ACCELERATION:
		{
			accelLinAccuracy = status;
			rawLinAccelX = data1;
			rawLinAccelY = data2;
			rawLinAccelZ = data3;
			break;
		}
		case SENSOR_REPORTID_GYROSCOPE:
		{
			gyroAccuracy = status;
			rawGyroX = data1;
			rawGyroY = data2;
			rawGyroZ = data3;
			break;
		}
		case SENSOR_REPORTID_MAGNETIC_FIELD:
		{
			magAccuracy = status;
			rawMagX = data1;
			rawMagY = data2;
			rawMagZ = data3;
			break;
		}
		case SENSOR_REPORTID_ROTATION_VECTOR:
		case SENSOR_REPORTID_GAME_ROTATION_VECTOR:
		{
			quatAccuracy = status;
			rawQuatI = data1;
			rawQuatJ = data2;
			rawQuatK = data3;
			rawQuatReal = data4;
			rawQuatRadianAccuracy = data5; //Only available on rotation vector, not game rot vector
			break;
		}
		case SENSOR_REPORTID_STEP_COUNTER:
		{
			stepCount = data3; //Bytes 8/9
			break;
		}
		case SENSOR_REPORTID_STABILITY_CLASSIFIER:
		{
			stabilityClassifier = shtpData[5 + 4]; //Byte 4 only
			break;
		}
		case SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER:
		{
			activityClassifier = shtpData[5 + 5]; //Most likely state

			//Load activity classification confidences into the array
			for (uint8_t x = 0; x < 9; x++)					   //Hardcoded to max of 9. TODO - bring in array size
				_activityConfidences[x] = shtpData[11 + x]; //5 bytes of timestamp, byte 6 is first confidence byte
			break;
		}
		case SHTP_REPORT_COMMAND_RESPONSE:
		{
			//printf("!");
			//The BNO080 responds with this report to command requests. It's up to use to remember which command we issued.
			uint8_t command = shtpData[5 + 2]; //This is the Command byte of the response

			if (command == COMMAND_ME_CALIBRATE)
			{
				//printf("ME Cal report found!");
				calibrationStatus = shtpData[5 + 5]; //R0 - Status (0 = success, non-zero = fail)
			}
			break;
		}
		default:
		{
			//This sensor report ID is unhandled.
			//See reference manual to add additional feature reports as needed
		}
	}

	//TODO additional feature reports may be strung together. Parse them all.
}

//Return the rotation vector quaternion I
float BNO080_getvalue(BNO080_tbl* p_sensor, getMode_t mode, getRaw_t raw)
{

  switch(mode)
  {
    case rawAccel:
    case rawLinAccel:
    case rawGyro:
    case rawMag:
    case rawQuatI:
    case rawQuatJ:
    case rawQuatK:
    case rawQuatReal:
        if(raw!=Accurancy)
        {
          BNO080_qToFloat
        }
  }
  BNO080
}
float BNO080_getQuatI()
{
	return BNO080_qToFloat(rawQuatI, rotationVector_Q1);
}

//Return the rotation vector quaternion J
float BNO080_getQuatJ()
{
	return BNO080_qToFloat(rawQuatJ, rotationVector_Q1);
}

//Return the rotation vector quaternion K
float BNO080_getQuatK()
{
	return BNO080_qToFloat(rawQuatK, rotationVector_Q1);
}

//Return the rotation vector quaternion Real
float BNO080_getQuatReal()
{
	return BNO080_qToFloat(rawQuatReal, rotationVector_Q1);
}

//Return the rotation vector accuracy
float BNO080_getQuatRadianAccuracy()
{
	return BNO080_qToFloat(rawQuatRadianAccuracy, rotationVector_Q1);
}

//Return the acceleration component
uint8_t BNO080_getQuatAccuracy()
{
	return (quatAccuracy);
}

//Return the acceleration component
float BNO080_getAccelX()
{
	return BNO080_qToFloat(rawAccelX, accelerometer_Q1);
}

//Return the acceleration component
float BNO080_getAccelY()
{
	return BNO080_qToFloat(rawAccelY, accelerometer_Q1);
}

//Return the acceleration component
float BNO080_getAccelZ()
{
	return BNO080_qToFloat(rawAccelZ, accelerometer_Q1);
}

//Return the acceleration component
uint8_t BNO080_getAccelAccuracy()
{
	return (accelAccuracy);
}

// linear acceleration, i.e. minus gravity

//Return the acceleration component
float BNO080_getLinAccelX()
{
	return BNO080_qToFloat(rawLinAccelX, linear_accelerometer_Q1);
}

//Return the acceleration component
float BNO080_getLinAccelY()
{
	return BNO080_qToFloat(rawLinAccelY, linear_accelerometer_Q1);
}

//Return the acceleration component
float BNO080_getLinAccelZ()
{
	return BNO080_qToFloat(rawLinAccelZ, linear_accelerometer_Q1);
}

//Return the acceleration component
uint8_t BNO080_getLinAccelAccuracy()
{
	return (accelLinAccuracy);
}

//Return the gyro component
float BNO080_getGyroX()
{
	return BNO080_qToFloat(rawGyroX, gyro_Q1);
}

//Return the gyro component
float BNO080_getGyroY()
{
	return BNO080_qToFloat(rawGyroY, gyro_Q1);
}

//Return the gyro component
float BNO080_getGyroZ()
{
	return BNO080_qToFloat(rawGyroZ, gyro_Q1);
}

//Return the gyro component
uint8_t BNO080_getGyroAccuracy()
{
	return (gyroAccuracy);
}

//Return the magnetometer component
float BNO080_getMagX()
{
	return BNO080_qToFloat(rawMagX, magnetometer_Q1);
}

//Return the magnetometer component
float BNO080_getMagY()
{
	return BNO080_qToFloat(rawMagY, magnetometer_Q1);
}

//Return the magnetometer component
float BNO080_getMagZ()
{
	return BNO080_qToFloat(rawMagZ, magnetometer_Q1);
}

//Return the mag component
uint8_t BNO080_getMagAccuracy()
{
	return (magAccuracy);
}

//Return the step count
uint16_t BNO080_getStepCount()
{
	return (stepCount);
}

//Return the stability classifier
uint8_t BNO080_getStabilityClassifier()
{
	return (stabilityClassifier);
}

//Return the activity classifier
uint8_t BNO080_getActivityClassifier()
{
	return (activityClassifier);
}

//Return the time stamp
uint32_t BNO080_getTimeStamp()
{
	return (timeStamp);
}

//Given a record ID, read the Q1 value from the metaData record in the FRS (ya, it's complicated)
//Q1 is used for all sensor data calculations
int16_t BNO080_getQ1(uint16_t recordID)
{
	//Q1 is always the lower 16 bits of word 7
	return BNO080_readFRSword(recordID, 7) & 0xFFFF; //Get word 7, lower 16 bits
}

//Given a record ID, read the Q2 value from the metaData record in the FRS
//Q2 is used in sensor bias
int16_t BNO080_getQ2(uint16_t recordID)
{
	//Q2 is always the upper 16 bits of word 7
	return BNO080_readFRSword(recordID, 7) >> 16; //Get word 7, upper 16 bits
}

//Given a record ID, read the Q3 value from the metaData record in the FRS
//Q3 is used in sensor change sensitivity
int16_t BNO080_getQ3(uint16_t recordID)
{
	//Q3 is always the upper 16 bits of word 8
	return BNO080_readFRSword(recordID, 8) >> 16; //Get word 8, upper 16 bits
}

//Given a record ID, read the resolution value from the metaData record in the FRS for a given sensor
float BNO080_getResolution(uint16_t recordID)
{
	//The resolution Q value are 'the same as those used in the sensor's input report'
	//This should be Q1.
	int16_t Q = BNO080_getQ1(recordID);

	//Resolution is always word 2
	uint32_t value = BNO080_readFRSword(recordID, 2); //Get word 2

	return BNO080_qToFloat(value, Q);
}

//Given a record ID, read the range value from the metaData record in the FRS for a given sensor
float BNO080_getRange(uint16_t recordID)
{
	//The resolution Q value are 'the same as those used in the sensor's input report'
	//This should be Q1.
	int16_t Q = BNO080_getQ1(recordID);

	//Range is always word 1
	uint32_t value = BNO080_readFRSword(recordID, 1); //Get word 1

	return BNO080_qToFloat(value, Q);
}

//Given a record ID and a word number, look up the word data
//Helpful for pulling out a Q value, range, etc.
//Use readFRSdata for pulling out multi-word objects for a sensor (Vendor data for example)
uint32_t BNO080_readFRSword(uint16_t recordID, uint8_t wordNumber)
{
	if (BNO080_readFRSdata(recordID, wordNumber, 1) == 1) //Get word number, just one word in length from FRS
		return (metaData[0]);						  //Return this one word

	return (0); //Error
}

//Ask the sensor for data from the Flash Record System
//See 6.3.6 page 40, FRS Read Request
void BNO080_frsReadRequest(uint16_t recordID, uint16_t readOffset, uint16_t blockSize)
{
	shtpData[0] = SHTP_REPORT_FRS_READ_REQUEST; //FRS Read Request
	shtpData[1] = 0;							//Reserved
	shtpData[2] = (readOffset >> 0) & 0xFF;		//Read Offset LSB
	shtpData[3] = (readOffset >> 8) & 0xFF;		//Read Offset MSB
	shtpData[4] = (recordID >> 0) & 0xFF;		//FRS Type LSB
	shtpData[5] = (recordID >> 8) & 0xFF;		//FRS Type MSB
	shtpData[6] = (blockSize >> 0) & 0xFF;		//Block size LSB
	shtpData[7] = (blockSize >> 8) & 0xFF;		//Block size MSB

	//Transmit packet on channel 2, 8 bytes
	BNO080_sendPacket(CHANNEL_CONTROL, 8);
}

//Given a sensor or record ID, and a given start/stop bytes, read the data from the Flash Record System (FRS) for this sensor
//Returns true if metaData array is loaded successfully
//Returns false if failure
int BNO080_readFRSdata(uint16_t recordID, uint8_t startLocation, uint8_t wordsToRead)
{
	uint8_t spot = 0;

	//First we send a Flash Record System (FRS) request
	BNO080_frsReadRequest(recordID, startLocation, wordsToRead); //From startLocation of record, read a # of words

	//Read bytes until FRS reports that the read is complete
	while (1)
	{
		//Now we wait for response
		while (1)
		{
			uint8_t counter = 0;
			while (BNO080_receivePacket() == 0)
			{
				if (counter++ > 100)
					return (0); //Give up
				HAL_Delay(1);
			}

			//We have the packet, inspect it for the right contents
			//See page 40. Report ID should be 0xF3 and the FRS types should match the thing we requested
			if (shtpData[0] == SHTP_REPORT_FRS_READ_RESPONSE)
				if (((uint16_t)shtpData[13] << 8 | shtpData[12]) == recordID)
					break; //This packet is one we are looking for
		}

		uint8_t dataLength = shtpData[1] >> 4;
		uint8_t frsStatus = shtpData[1] & 0x0F;

		uint32_t data0 = (uint32_t)shtpData[7] << 24 | (uint32_t)shtpData[6] << 16 | (uint32_t)shtpData[5] << 8 | (uint32_t)shtpData[4];
		uint32_t data1 = (uint32_t)shtpData[11] << 24 | (uint32_t)shtpData[10] << 16 | (uint32_t)shtpData[9] << 8 | (uint32_t)shtpData[8];

		//Record these words to the metaData array
		if (dataLength > 0)
		{
			metaData[spot++] = data0;
		}
		if (dataLength > 1)
		{
			metaData[spot++] = data1;
		}

		if (spot >= MAX_METADATA_SIZE)
		{
			printf("metaData array over run. Returning.");
			return (1); //We have run out of space in our array. Bail.
		}

		if (frsStatus == 3 || frsStatus == 6 || frsStatus == 7)
		{
			return (1); //FRS status is read completed! We're done!
		}
	}
}

//Send command to reset IC
//Read all advertisement packets from sensor
//The sensor has been seen to reset twice if we attempt too much too quickly.
//This seems to work reliably.
void BNO080_softReset(void)
{
	shtpData[0] = 1; //Reset

	//Attempt to start communication with sensor
	BNO080_sendPacket(CHANNEL_EXECUTABLE, 1); //Transmit packet on channel 1, 1 byte

	//Read all incoming data and flush it
	HAL_Delay(50);
	while (BNO080_receivePacket() == 1);
	HAL_Delay(50);
	while (BNO080_receivePacket() == 1);
}

//Get the reason for the last reset
//1 = POR, 2 = Internal reset, 3 = Watchdog, 4 = External reset, 5 = Other
uint8_t BNO080_resetReason()
{
	shtpData[0] = SHTP_REPORT_PRODUCT_ID_REQUEST; //Request the product ID and reset info
	shtpData[1] = 0;							  //Reserved

	//Transmit packet on channel 2, 2 bytes
	BNO080_sendPacket(CHANNEL_CONTROL, 2);

	//Now we wait for response
	if (BNO080_receivePacket() == 1)
	{
		if (shtpData[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE)
		{
			return (shtpData[1]);
		}
	}

	return (0);
}

//Given a register value and a Q point, convert to float
//See https://en.wikipedia.org/wiki/Q_(number_format)
float BNO080_qToFloat(int16_t fixedPointValue, uint8_t qPoint)
{
	return fixedPointValue * powf(2, qPoint * -1);
}

bool BNO080_set_report(Sensor_Mode_t mode, uint16_t timeBetweenReports)
{
  BNO080_setFeatureCommand(mode, timeBetweenReports, 0);
  return true;
}

bool BNO080_calibrateSet(Calibrate_Mode_t mode)
{
  bool ret=false;
  if(mode<CALIBRATE_MAX)
  {
    BNO080_sendCalibrateCommand(mode);
    ret=true;
  }
  return ret;
}
//See page 51 of reference manual - ME Calibration Response
//Byte 5 is parsed during the readPacket and stored in calibrationStatus
bool BNO080_calibrationComplete(void)
{
  bool ret=false;
  BNO080_tbl* p_sensor = &BNO080;
	if(p_sensor->calibrationStatus== 0)
	{
	  ret=true;
	}
	return ret;
}

//Given a sensor's report ID, this tells the BNO080 to begin reporting the values
//Also sets the specific config word. Useful for personal activity classifier
void BNO080_setFeatureCommand(uint8_t reportID, uint32_t microsBetweenReports, uint32_t specificConfig)
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
  p_packet->shtpData[0] = SHTP_REPORT_SET_FEATURE_COMMAND;	 //Set feature command. Reference page 55
  p_packet->shtpData[1] = reportID;						 //Feature Report ID. 0x01 = Accelerometer, 0x05 = Rotation vector
  p_packet->shtpData[2] = 0;							 //Feature flags
  p_packet->shtpData[3] = 0;							 //Change sensitivity (LSB)
  p_packet->shtpData[4] = 0;							 //Change sensitivity (MSB)
  p_packet->shtpData[5] = (microsBetweenReports >> 0) & 0xFF;  //Report interval (LSB) in microseconds. 0x7A120 = 500ms
  p_packet->shtpData[6] = (microsBetweenReports >> 8) & 0xFF;  //Report interval
  p_packet->shtpData[7] = (microsBetweenReports >> 16) & 0xFF; //Report interval
  p_packet->shtpData[8] = (microsBetweenReports >> 24) & 0xFF; //Report interval (MSB)
  p_packet->shtpData[9] = 0;							 //Batch Interval (LSB)
  p_packet->shtpData[10] = 0;							 //Batch Interval
  p_packet->shtpData[11] = 0;							 //Batch Interval
  p_packet->shtpData[12] = 0;							 //Batch Interval (MSB)
  p_packet->shtpData[13] = (specificConfig >> 0) & 0xFF;	   	 //Sensor-specific config (LSB)
  p_packet->shtpData[14] = (specificConfig >> 8) & 0xFF;	   	 //Sensor-specific config
  p_packet->shtpData[15] = (specificConfig >> 16) & 0xFF;	 //Sensor-specific config
  p_packet->shtpData[16] = (specificConfig >> 24) & 0xFF;	 //Sensor-specific config (MSB)

	//Transmit packet on channel 2, 17 bytes
	BNO080_sendPacket(CHANNEL_CONTROL, 17);
}

//Tell the sensor to do a command
//See 6.3.8 page 41, Command request
//The caller is expected to set P0 through P8 prior to calling
void BNO080_sendCommand(uint8_t command)
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
  p_packet->shtpData[0] = SHTP_REPORT_COMMAND_REQUEST; //Command Request
  p_packet->shtpData[1] = p_packet->commandSequenceNumber++;	 //Increments automatically each function call
  p_packet->shtpData[2] = command;					   //Command

	//Caller must set these
	/*shtpData[3] = 0; //P0
	shtpData[4] = 0; //P1
	shtpData[5] = 0; //P2
	shtpData[6] = 0;
	shtpData[7] = 0;
	shtpData[8] = 0;
	shtpData[9] = 0;
	shtpData[10] = 0;
	shtpData[11] = 0;*/

	//Transmit packet on channel 2, 12 bytes
	BNO080_sendPacket(CHANNEL_CONTROL, 12);
}

//This tells the BNO080 to begin calibrating
//See page 50 of reference manual and the 1000-4044 calibration doc
/*shtpData[3] = 0; //P0 - Accel Cal Enable
  shtpData[4] = 0; //P1 - Gyro Cal Enable
  shtpData[5] = 0; //P2 - Mag Cal Enable
  shtpData[6] = 0; //P3 - Subcommand 0x00
  shtpData[7] = 0; //P4 - Planar Accel Cal Enable
  shtpData[8] = 0; //P5 - Reserved
  shtpData[9] = 0; //P6 - Reserved
  shtpData[10] = 0; //P7 - Reserved
  shtpData[11] = 0; //P8 - Reserved*/
void BNO080_sendCalibrateCommand(uint8_t thingToCalibrate)
{
  BNO080_tbl* p_sensor = &BNO080;
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
	for (uint8_t i = 3; i < 12; i++) //Clear this section of the shtpData array
	{
	  p_packet->shtpData[i] = 0;
	}
	switch(thingToCalibrate)
	{
	  case CALIBRATE_ACCEL:
	    p_packet->shtpData[3] = 1;
	    break;
	  case CALIBRATE_GYRO:
	    p_packet->shtpData[4] = 1;
	    break;
	  case CALIBRATE_MAG:
	    p_packet->shtpData[5] = 1;
	    break;
	  case CALIBRATE_PLANAR_ACCEL:
	    p_packet->shtpData[7] = 1;
	    break;
	  case CALIBRATE_ACCEL_GYRO_MAG:
	    p_packet->shtpData[3] = 1;
	    p_packet->shtpData[4] = 1;
	    p_packet->shtpData[5] = 1;
	    break;
	  case CALIBRATE_STOP:
	    break;
	  default :
	    break;
	} //Do nothing, bytes are set to zero

	//Make the internal calStatus variable non-zero (operation failed) so that user can test while we wait
	p_sensor->calibrationStatus = 1;

	//Using this shtpData packet, send a command
	BNO080_sendCommand(COMMAND_ME_CALIBRATE);
}

//Request ME Calibration Status from BNO080
//See page 51 of reference manual
/*shtpData[3] = 0; //P0 - Reserved
  shtpData[4] = 0; //P1 - Reserved
  shtpData[5] = 0; //P2 - Reserved
  shtpData[6] = 0; //P3 - 0x01 - Subcommand: Get ME Calibration
  shtpData[7] = 0; //P4 - Reserved
  shtpData[8] = 0; //P5 - Reserved
  shtpData[9] = 0; //P6 - Reserved
  shtpData[10] = 0; //P7 - Reserved
  shtpData[11] = 0; //P8 - Reserved*/

void BNO080_requestCalibrationStatus()
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
	for (uint8_t i = 3; i < 12; i++) //Clear this section of the shtpData array
	{
	  p_packet->shtpData[i] = 0;
	}
	p_packet->shtpData[6] = 0x01; //P3 - 0x01 - Subcommand: Get ME Calibration

	//Using this shtpData packet, send a command
	BNO080_sendCommand(COMMAND_ME_CALIBRATE);
}

//This tells the BNO080 to save the Dynamic Calibration Data (DCD) to flash
//See page 49 of reference manual and the 1000-4044 calibration doc
/*shtpData[3] = 0; //P0 - Reserved
  shtpData[4] = 0; //P1 - Reserved
  shtpData[5] = 0; //P2 - Reserved
  shtpData[6] = 0; //P3 - Reserved
  shtpData[7] = 0; //P4 - Reserved
  shtpData[8] = 0; //P5 - Reserved
  shtpData[9] = 0; //P6 - Reserved
  shtpData[10] = 0; //P7 - Reserved
  shtpData[11] = 0; //P8 - Reserved*/
void BNO080_saveCalibration()
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
	for (uint8_t i = 3; i < 12; i++) //Clear this section of the shtpData array
	{
	  p_packet->shtpData[i] = 0;
	}   //Using this shtpData packet, send a command
	BNO080_sendCommand(COMMAND_DCD); //Save DCD command
}

//Blocking wait for BNO080 to assert (pull low) the INT pin
//indicating it's ready for comm. Can take more than 104ms
//after a hardware reset
bool BNO080_waitForSPI(void)
{
  for (uint32_t counter = 0; counter < 0xffffffff; counter++) //Don't got more than 255
  {
    if (HAL_GPIO_ReadPin(BNO080_INT_PORT, BNO080_INT_PIN) == GPIO_PIN_RESET)
    {
      return true;
    }
  }
  return false;
}



bool BNO080_receivePacket(void)
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
  bool ret=false;
	uint8_t incoming;
	uint8_t zero=0x00;
	uint8_t full=0xFF;
	if (HAL_GPIO_ReadPin(BNO080_INT_PORT, BNO080_INT_PIN) == GPIO_PIN_SET)
	{
	  return false;
	}
	CHIP_SELECT(BNO080);// CS LOW
	//Get the first four bytes, aka the packet header
  for(int i=0; i<4; i++)
  {
    BNO080_TxRxByte(&full, &p_packet->shtpHeader[i]);
  }
	//Calculate the number of data bytes in this packet
	int16_t dataLength = ((uint16_t)p_packet->shtpHeader[0] << 8 | p_packet->shtpHeader[1]);
	dataLength &= 0x7fff; //Clear the MSbit.
	//This bit indicates if this package is a continuation of the last. Ignore it for now.
	//TODO catch this as an error and exit
	if (dataLength == 0)
	{
		return false;
	}
	dataLength -= 4; //Remove the header bytes from the data count

	for (uint16_t dataSpot = 0; dataSpot < dataLength; dataSpot++)
	{
		BNO080_TxRxByte(&full, &incoming);
		if (dataSpot < MAX_PACKET_SIZE)	//BNO080 can respond with upto 270 bytes, avoid overflow
		{
		  p_packet->shtpHeader[dataSpot] = incoming; //Store data into the shtpData array
		}
	}
	CHIP_DESELECT(BNO080); //Release BNO080
	return (1); //We're done!
}


//TODO - Arduino has a max 32 byte send. Break sending into multi packets if needed.
int BNO080_sendPacket(uint8_t channelNumber, uint8_t dataLength)
{
  BNO080_Packet_tbl* p_packet = &BNO080.BNO080_Packet;
	uint8_t packetLength = dataLength + 4; //Add four bytes for the header
	                                        //Wait for BNO080 to indicate it is available for communication
	if (BNO080_waitForSPI() == 0)
	{
	  return (0);
	}
	CHIP_SELECT(BNO080);
	                                         //Send the 4 byte packet header
	SPI2_SendByte(packetLength & 0xFF);			//Packet length LSB
	SPI2_SendByte(packetLength >> 8);				//Packet length MSB
	SPI2_SendByte(channelNumber);					//Channel number
	SPI2_SendByte(p_packet->sequenceNumber[channelNumber]++); 	//Send the sequence number, increments with each packet sent, different counter for each channel

	for (uint8_t i = 0; i < dataLength; i++)
	{
		SPI2_SendByte(p_packet->shtpData[i]);
	}

	CHIP_DESELECT(BNO080);

	return (1);
}



