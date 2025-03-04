/*
� Namawell 
 * RFID demo project

 * This file contains all the functions that are not generated by the MCC
*/
#include "functions.h"
#include "rfid.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/uart/uart1.h"
#include "mcc_generated_files/uart/uart2.h"
#include "mcc_generated_files/spi_host/spi1.h"

/*
 functions list:
 * 
 * 
*/
    uint8_t sensorData[16];
    uint8_t dummyBuffer[16];  // To send dummy bytes for reading
    uint8_t bma400ID;
    
void shortDelay(uint16_t time)     // delay a number of Fosc cycles. Not accurate
{
    while (time > 0)
    {
        time--;
    }
}
void timedAppTasks(void)
{
    if (pgv->UART1Flags & fUART1_Txing)
    {
        UART1_SendMessage();
    }
    
    if (pgv->timerFlags & ftimer_01ms)
    {
        timedEvents_01ms();
    }
    
    if (pgv->timerFlags & ftimer_05ms)
    {
        timedEvents_05ms();
    }
    if (pgv->timerFlags & ftimer_10ms)
    {
        timedEvents_10ms();     //To read sensor data and send them to UART1
    }
    
    if (pgv->timerFlags & ftimer_50ms)
    {
        timedEvents_50ms();     //To read sensor data and send them to UART1
    }    
    
    if (pgv->timerFlags & ftimer_100ms)
    {
        timedEvents_100ms();     //To read tag data and send them to UART1
    }

    if (pgv->UART1Flags & fUART1_RxMsgReady)
    {
        
        UART1_RxMessageProcess();
    }
}


void TMR1_TimeoutCallback( void )       //Timer1 interrupt service - 1ms
{
    pgv->timerFlags |= ftimer_01ms;
    
    if (pgv->delayTimeCounter > 0)      //The delay function must be inside the ISR
    {
        pgv->delayTimeCounter--;
    }
}

void timedEvents_01ms(void)
{
    pgv->timerFlags &= ~ftimer_01ms;
           
    //--- Timed events
    pgv->timerCnt_05ms++;
    if (pgv->timerCnt_05ms == 5)
    {
        pgv->timerCnt_05ms = 0;
        pgv->timerFlags |= ftimer_05ms;
    }
    
    pgv->timerCnt_10ms++;
    if (pgv->timerCnt_10ms == 10)
    {
        pgv->timerCnt_10ms = 0;
        pgv->timerFlags |= ftimer_10ms;
    }
    
    pgv->timerCnt_50ms++;
    if (pgv->timerCnt_50ms == 50)
    {
        pgv->timerCnt_50ms = 0;
        pgv->timerFlags |= ftimer_50ms;
    }
    
    pgv->timerCnt_100ms++;
    if (pgv->timerCnt_100ms == 100)
    {
        pgv->timerCnt_100ms = 0;
        pgv->timerFlags |= ftimer_100ms;
    }  

}

void timedEvents_05ms(void)
{
    pgv->timerFlags &= ~ftimer_05ms;
    // LATDbits.LATD10 ^= 1;
} 

void timedEvents_10ms(void)
{
    pgv->timerFlags &= ~ftimer_10ms;

}

void timedEvents_50ms(void)
{
    pgv->timerFlags &= ~ftimer_50ms;
 
}

void timedEvents_100ms(void)
{
    pgv->timerFlags &= ~ftimer_100ms;

    if (MFRC522_DetectTag() == MI_OK)
    {
        // A tag is detected, and its serial number (ID) is read
        // Process the tag data here
        // Send the SN to PC
        pgv->txBuffer[0] = MSG_HEADER_OUT;    
        pgv->txBuffer[1] = MSG_TYPE_SN;    
        pgv->txBuffer[2] = pgv->tagSN[0];    
        pgv->txBuffer[3] = pgv->tagSN[1];    
        pgv->txBuffer[4] = pgv->tagSN[2];    
        pgv->txBuffer[5] = pgv->tagSN[3];    

        UART1_SendMsgPrepare(6);

        LATDbits.LATD10 ^= 1;
    }

}

void IO_RD13_Btn_CallBack(void)
{
    static uint8_t blockAddr = 0;
    LATDbits.LATD10 ^= 1;         // Turn off the LED on the controller board
/*    
    if (MFRC522_DetectTag() == MI_OK)
    {
        // A tag is detected, and its serial number (ID) is read
        // Process the tag data here
        // Send the SN to PC
        pgv->txBuffer[0] = MSG_HEADER_OUT;    
        pgv->txBuffer[1] = MSG_TYPE_SN;    
        pgv->txBuffer[2] = pgv->tagSN[0];    
        pgv->txBuffer[3] = pgv->tagSN[1];    
        pgv->txBuffer[4] = pgv->tagSN[2];    
        pgv->txBuffer[5] = pgv->tagSN[3];    

        UART1_SendMsgPrepare(6);

        LATDbits.LATD10 ^= 1;

    //    MFRC522_Read(blockAddr++);        // Read block data to tagRxData[]
    //    blockAddr &= 0x3F;            // circulation from 0 - 63  
    }
 * */
}


void myDelay(uint16_t delayTime)    // delay Time in ms. using timer1
{
    pgv->delayTimeCounter = delayTime;
    while (pgv->delayTimeCounter);   //delay time is counted in the TimeoutCallback 
}

void UART1_SendMsgPrepare(uint8_t bytes)
{
    pgv->bytesToTx = bytes;
    pgv->txBufferIndex = 0;
    pgv->UART1Flags |= fUART1_Txing;

}

void UART1_SendMessage(void)
{
    if (pgv->bytesToTx > 0)
    {
        if (UART1_IsTxReady())    //If it is not ready, Do not wait here.
        {
            UART1_Write(pgv->txBuffer[pgv->txBufferIndex]);
    //        pgv->UART1Flags &= ~fUART1_TxReady;      // To be set in TxCompleteCallBack())
            pgv->txBufferIndex++;
            pgv->bytesToTx--;
            if (pgv->bytesToTx == 0)
            {
                pgv->UART1Flags &= ~fUART1_Txing;
            }
        }
    }
}

void UART1_TxCompleteCallback(void)
{
//    pgv->UART1Flags |= fUART1_TxReady;
    
}

//----- How many data bytes to receive, needs to be defined.
void UART1_RxCompleteCallback(void)
{ 
    pgv->rxBuffer[pgv->rxBufferIndex++] = UART1_Read();
    if (pgv->rxBufferIndex>=2)
    {
        pgv->UART1Flags |= fUART1_RxMsgReady;
    }
    
} 

void UART1_RxMessageProcess(void)
{
    pgv->UART1Flags &= ~fUART1_RxMsgReady;
    pgv->rxBufferIndex = 0;
    if ((pgv->rxBuffer[0] == 0x55) && (pgv->rxBuffer[1] == 0xAA))
    {
        LATDbits.LATD10 ^= 1;        //Place holder here
    }
}


//----- SPI functions -----------
// Function to initialize CS pin
void BMA400_CS_Init(void)
{
    PORTCbits.RC6 = 0;  // to power off the sensor
    myDelay(200);       // to delay xxms for the sensor to reset
    PORTCbits.RC6 = 1;  // to power on the sensor
    myDelay(300);
    
    if (!SPI1_Open(0))
    {
        LATDbits.LATD10 = 0;     // turn on Led
    }
    
    SPI1_ByteExchange(BMA400_ID_READ); // dummy read to establish the SPI
    
    myDelay(10);
    
    BMA400_WriteRegister(0x19, 0x02);   //Switch the sensor to Normal mode   
    
    BMA400_WriteRegister(0x19, 0x02);   //Switch the sensor to Normal mode
    BMA400_WriteRegister(0x1A, 0x38);
    BMA400_WriteRegister(0x1B, 0x04);
    
    myDelay(200);
}

// Function to read the sensor ID
uint8_t BMA400_ReadID(void)
{
    uint8_t id;

    PORTCbits.RC3 = 0; // Select the sensor (CS low)

    SPI1_ByteExchange(BMA400_ID_READ); // Send read command
    SPI1_ByteExchange(SPI1_DUMMY_DATA_BMA400); // dummy Read ID
    id = SPI1_ByteExchange(SPI1_DUMMY_DATA_BMA400); // Read ID
    
    PORTCbits.RC3 = 1; // Deselect the sensor (CS high)

    return id;
}

void BMA400_ReadRegisters(uint8_t startAddress, uint8_t *data, uint8_t length)
{
    uint8_t command = startAddress | 0x80;  // 0x80 to set the read bit in the command
    // Select the sensor (pull CS low)
    PORTCbits.RC3 = 0; // Select the sensor (CS low)

    // Send the command to start reading from the specified register
    SPI1_ByteExchange(command);
    SPI1_ByteExchange(SPI1_DUMMY_DATA_BMA400); // dummy Read ID, ignore the first byte
    // Perform the SPI transfer to read multiple registers
    SPI1_BufferExchange(dummyBuffer, length);  // Send dummy bytes, receive data

    // Copy received data to the output buffer
    for (uint8_t i = 0; i < length; i++) {
        data[i] = dummyBuffer[i];
    }

    // Deselect the sensor (pull CS high)
    PORTCbits.RC3 = 1; // 
}

void BMA400_WriteRegister(uint8_t regAddress, uint8_t data)
{
    // Set the chip select line low to start communication
    // Select the sensor (pull CS low)
    PORTCbits.RC3 = 0; // Select the sensor (CS low)
    
    // Send the register address with write operation (MSB = 0)
    SPI1_ByteExchange(regAddress);
    
    // Send the data byte to be written to the register
    SPI1_ByteExchange(data);
    
    // Deselect the sensor (pull CS high)
    PORTCbits.RC3 = 1; // 
}