#include "config.h"
#include <stdio.h>
#include "PIC16F877A_I2C.h"
#include "PIC16F877A_timer0.h"
#include "PIC16F877A_HCSR04.h"
#include "PIC16F877A_UART.h"

// Define the I2C address of this MCU
#define I2C_ADDRESS 0x10

// Global variable
float obs_distance = 0;
char distance_command = 'a';
uint8_t count = 0;
char tx_buffer[15];

// Function to handle the interrupt
void __interrupt() ISR(void)
{
    // Trigger the ultrasonic sensor every 10 milliseconds
    if(TMR0IF)
    {
        TMR0 = 6;
        if(count++ == 20)
        {
            count = 0;
            hcsr04Trigger();
        }
        TMR0IF = 0;
    }
    // Get the distance of the ultrasonic sensor
    if(CCP1IF)
    {
        if(hcsr04Distance())
        {
            obs_distance = getDistance();
            // Divide the case of the distance
            if(obs_distance > 100)
                distance_command = 'a';
            else if(obs_distance < 50)
                distance_command = 'c';
            else 
                distance_command = 'b';
        }
        CCP1IF = 0;
    }
    // I2C communication 
    if(SSPIF)
    {
        // Error handling
        if(WCOL || SSPOV)
        {
            // Read the data
            char dummy = SSPBUF;
            // Clear the error flag bits
            WCOL = 0;
            SSPOV = 0;
            // Release the clock line
            SSPCONbits.CKP = 1;
        }
        // Case of slave transmitter required
        if(R_nW)
        {
            // Read the data
            char dummy = SSPBUF;
            // Transmit the right data
            SSPBUF = distance_command;
            // Release the clock line
            SSPCONbits.CKP = 1;
            // Wait for transmit to be completed
            while(SSPSTATbits.BF);
        }
        SSPIF = 0;
    }
}

void main(void) 
{
    // Initiate the I2C in slave transmitter mode
    I2C_Slave_Init(I2C_ADDRESS);
    // Initiate the Timer 0 TIMER mode
    TMR0 = 6;
    timer0TimerInit(TIMER0_DIV_16);
    // Initiate the HCSR04 ultrasonic sensor
    hcsr04Init();
    // Initiate the UART module
    UARTTransInit();
    
    while(1)
    {
//        sprintf(tx_buffer,"Dust: %.2f cm\r\n", obs_distance);
//        UARTsendString(tx_buffer);
//        __delay_ms(50);
    }
    return;
}
