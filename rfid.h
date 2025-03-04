
#ifndef RFID_H
#define	RFID_H

#include <xc.h> 
//#include <stdint.h>
//#include <stdbool.h>
//#include <stddef.h>

#define MFRC522_RESET_PIN LATCbits.LATC4  // RC4 for Reset

#define TOTAL_BLOCKS 64  // Total blocks in a Classic 1K tag
#define BLOCK_SIZE 16     // Each block is 16 bytes

//  #define MFRC552                  0x28   // chip 0101b ADDRESS
#define CommandReg				 0x01 	// starts and stops command execution
#define ComIEnReg				 0x02 	// enable and disable interrupt request control bits
#define DivIEnReg				 0x03 	// enable and disable interrupt request control bits
#define ComIrqReg				 0x04 	// interrupt request bits
#define DivIrqReg				 0x05 	// interrupt request bits
#define ErrorReg				 0x06 	// error bits showing the error status of the last command executed
#define Status1Reg				 0x07 	// communication status bits
#define Status2Reg				 0x08 	// receiver and transmitter status bits
#define FIFODataReg				 0x09 	// input and output of 64 byte FIFO buffer
#define FIFOLevelReg			 0x0A 	// number of bytes stored in the FIFO buffer
#define WaterLevelReg			 0x0B 	// level for FIFO underflow and overflow warning
#define ControlReg				 0x0C 	// miscellaneous control registers
#define BitFramingReg			 0x0D 	// adjustments for bit-oriented frames
#define CollReg					 0x0E 	// bit position of the first bit-collision detected on the RF interface

// Page 1: Command
#define ModeReg					 0x11 	// defines general modes for transmitting and receiving
#define TxModeReg				 0x12 	// defines transmission data rate and framing
#define RxModeReg				 0x13 	// defines reception data rate and framing
#define TxControlReg			 0x14 	// controls the logical behavior of the antenna driver pins TX1 and TX2
#define TxASKReg				 0x15 	// controls the setting of the transmission modulation
#define TxSelReg				 0x16 	// selects the internal sources for the antenna driver
#define RxSelReg				 0x17 	// selects internal receiver settings
#define RxThresholdReg			 0x18 	// selects thresholds for the bit decoder
#define DemodReg				 0x19 	// defines demodulator settings
#define MfTxReg					 0x1C 	// controls some MIFARE communication transmit parameters
#define MfRxReg					 0x1D 	// controls some MIFARE communication receive parameters
#define SerialSpeedReg			 0x1F 	// selects the speed of the serial UART interface

// Page 2: Configuration
#define CRCResultRegM			 0x21 	// shows the MSB and LSB values of the CRC calculation
#define CRCResultRegL			 0x22 
#define ModWidthReg				 0x24 	// controls the ModWidth setting?
#define RFCfgReg				 0x26 	// configures the receiver gain
#define GsNReg					 0x27 	// selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define CWGsPReg				 0x28 	// defines the conductance of the p-driver output during periods of no modulation
#define ModGsPReg				 0x29 	// defines the conductance of the p-driver output during periods of modulation
#define TModeReg				 0x2A 	// defines settings for the internal timer
#define TPrescalerReg			 0x2B 	// the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
#define TReloadRegH				 0x2C 	// defines the 16-bit timer reload value
#define TReloadRegL				 0x2D 
#define TCounterValueRegH		 0x2E 	// shows the 16-bit timer value
#define TCounterValueRegL		 0x2F 

// Page 3: Test Registers
#define TestSel1Reg				 0x31 	// general test signal configuration
#define TestSel2Reg				 0x32 	// general test signal configuration
#define TestPinEnReg			 0x33 	// enables pin output driver on pins D1 to D7
#define TestPinValueReg			 0x34 	// defines the values for D1 to D7 when it is used as an I/O bus
#define TestBusReg				 0x35 	// shows the status of the internal test bus
#define AutoTestReg				 0x36 	// controls the digital self test
#define VersionReg				 0x37 	// shows the software version
#define AnalogTestReg			 0x38 	// controls the pins AUX1 and AUX2
#define TestDAC1Reg				 0x39 	// defines the test value for TestDAC1
#define TestDAC2Reg				 0x3A 	// defines the test value for TestDAC2
#define TestADCReg				 0x3B 		// shows the value of ADC I and Q channels

// MFRC522 commands. Described in chapter 10 of the datasheet.
#define PCD_IDLE                 0x00   //NO action; Cancel the current command
#define PCD_Mem					 0x01		// stores 25 bytes into the internal buffer
#define PCD_GenerateRandomID	 0x02		// generates a 10-byte random ID number
#define PCD_CALCCRC				 0x03		// activates the CRC coprocessor or performs a self test
#define PCD_Transmit			 0x04		// transmits data from the FIFO buffer
#define PCD_NoCmdChange			 0x07		// no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
#define PCD_Receive				 0x08		// activates the receiver circuits
#define PCD_TRANSCEIVE 			 0x0C		// transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
#define PCD_AUTHENT 			 0x0E		// performs the MIFARE standard authentication as a reader
#define PCD_RESETPHASE           0x0F       //Reset

// MFRC522 RxGain[2:0] masks, defines the receiver's signal voltage gain factor (on the PCD). Described in 9.3.3.6 / table 98 of the datasheet
#define RxGain_18dB				 0x00 << 4	// 000b - 18 dB, minimum
#define RxGain_23dB				 0x01 << 4	// 001b - 23 dB
#define RxGain_18dB_2			 0x02 << 4	// 010b - 18 dB, it seems 010b is a duplicate for 000b
#define RxGain_23dB_2			 0x03 << 4	// 011b - 23 dB, it seems 011b is a duplicate for 001b
#define RxGain_33dB				 0x04 << 4	// 100b - 33 dB, average, and typical default
#define RxGain_38dB				 0x05 << 4	// 101b - 38 dB
#define RxGain_43dB				 0x06 << 4	// 110b - 43 dB
#define RxGain_48dB				 0x07 << 4	// 111b - 48 dB, maximum
#define RxGain_min				 0x00 << 4	// 000b - 18 dB, minimum, convenience for RxGain_18dB
#define RxGain_avg				 0x04 << 4	// 100b - 33 dB, average, convenience for RxGain_33dB
#define RxGain_max				 0x07 << 4		// 111b - 48 dB, maximum, convenience for RxGain_48dB

// Commands sent to the PICC.
// The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
#define PICC_REQIDL              0x26		// REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
#define PICC_READ                0x30       // Read Block
#define PICC_CMD_WUPA			 0x52		// Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
#define PICC_CMD_CT				 0x88		// Cascade Tag. Not really a command, but used during anti collision.
#define PICC_ANTICOLL            0x93       // anti-collision#define PICC_CMD_SEL_CL2	
#define PICC_CMD_SEL_CL3		 0x97		// Anti collision/Select, Cascade Level 3
#define PICC_HALT                0x50		// HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
#define PICC_SELECTTAG           0x93       // election card

// The commands used for MIFARE Classic (from http://www.nxp.com/documents/data_sheet/MF1S503x.pdf, Section 9)
// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
// The read/write commands can also be used for MIFARE Ultralight.
#define PICC_CMD_MF_AUTH_KEY_A	 0x60		// Perform authentication with Key A
#define PICC_CMD_MF_AUTH_KEY_B	 0x61		// Perform authentication with Key B
#define PICC_CMD_MF_READ		 0x30		// Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
#define PICC_WRITE               0xA0		// Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
#define PICC_CMD_MF_DECREMENT	 0xC0		// Decrements the contents of a block and stores the result in the internal data register.
#define PICC_CMD_MF_INCREMENT	 0xC1		// Increments the contents of a block and stores the result in the internal data register.
#define PICC_CMD_MF_RESTORE		 0xC2		// Reads the contents of a block into the internal data register.
#define PICC_CMD_MF_TRANSFER	 0xB0		// Writes the contents of the internal data register to a block.

// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
#define PICC_CMD_UL_WRITE		 0xA2		// Writes one 4 byte page to the PICC.
#define MAX_LEN 16

// status stuff And MF522 The error code is returned when communication
#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2


void MFRC522_Init(void);
void PCD_WriteRegister(	uint8_t reg, uint8_t data );
uint8_t PCD_ReadRegister( uint8_t reg );
//void PCD_ReadRegister( uint8_t reg, uint8_t values , uint8_t count);

void SetBitMask(uint8_t reg, uint8_t mask);
void ClearBitMask(uint8_t reg, uint8_t mask);
void AntennaOn(void);
void AntennaOff(void);
void MFRC522_Reset(void);
void MFRC522_EnableInterrupt(void);

uint8_t MFRC522_DetectTag(void);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen);
uint8_t MFRC522_Anticoll(uint8_t *serNum);
void CalculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);
uint8_t MFRC522_SelectTag(uint8_t *serNum);

uint8_t MFRC522_Authenticate(uint8_t blockAddr, uint8_t *key, uint8_t *serNum);
uint8_t MFRC522_WriteBlock(uint8_t blockAddr);
uint8_t MFRC522_ReadBlock(uint8_t blockAddr);

void MFRC522_Halt(void);

void MFRC522_ReadAllBlocks(void);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

