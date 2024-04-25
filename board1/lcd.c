#include "i2c.h"
#include "lcd.h"
#include "msp430g2553.h"

void LcdInit()
{
     // Initialize LCD
    LcdWriteCommand(LCD_INIT_BYTE, 0); 
    __delay_cycles(80000);
    LcdWriteCommand(LCD_INIT_BYTE, 0);
    LcdWriteCommand(LCD_INIT_BYTE, 0);
    // Configure LCD settings
    LcdWriteCommand(LCD_BUS_WIDTH_4BIT, 0);
    LcdWriteCommand(LCD_4BITS_2LINES_5x8FONT, 1);
    LcdWriteCommand(LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF, 1);
    LcdWriteCommand(LCD_CLEAR, 1);
    LcdWriteCommand(LCD_INCREMENT_NO_SHIFT, 1);
    LcdWriteCommand(LCD_DISPLAY_ON_CURSOR_OFF, 1);
}

void LcdWriteCommand(unsigned char data, unsigned char cmdtype)
{
    unsigned char byte;

    byte = (HI_NIBBLE(data) | LCD_BL) | LCD_EN; // Construct command byte for I2C transmission
    I2cTransmit(0x27, byte); // Transmit command byte via I2C
    while(I2cNotReady()); // Wait for I2C transmission to complete

    byte = (HI_NIBBLE(data) | LCD_BL) & ~LCD_EN;    // Construct command byte for I2C transmission
    I2cTransmit(0x27, byte); // Transmit command byte via I2C
    while(I2cNotReady()); // Wait for I2C transmission to complete

    // cmdtype = 0 -> One write cycle
    // cmdtype = 1 -> Two write cycles (4 bit mode)

    if (cmdtype) // If cmdtype is 1 (4-bit mode), repeat the above steps for the low nibble of the command
    {
        byte = (LO_NIBBLE(data) | LCD_BL) | LCD_EN;
        I2cTransmit(0x27, byte);
        while(I2cNotReady());
        byte = (LO_NIBBLE(data) | LCD_BL) | ~LCD_EN;
        I2cTransmit(0x27, byte);
        while(I2cNotReady());
    }

    __delay_cycles(80000); // Delay for stabilization
}
void LcdWriteChar(unsigned char data)
{
    unsigned char byte;

    byte = (HI_NIBBLE(data) | LCD_BL | LCD_RS) | LCD_EN; // Construct data byte for I2C transmission
    I2cTransmit(0x27, byte); // Transmit data byte via I2C
    while(I2cNotReady()); // Wait for I2C transmission to complete

    byte = (HI_NIBBLE(data) | LCD_BL | LCD_RS) & ~LCD_EN;// Construct data byte for I2C transmission
    I2cTransmit(0x27, byte);// Transmit data byte via I2C
    while(I2cNotReady());// Wait for I2C transmission to complete

    byte = (LO_NIBBLE(data) | LCD_BL | LCD_RS) | LCD_EN;// Construct data byte for I2C transmission
    I2cTransmit(0x27, byte);// Transmit data byte via I2C
    while(I2cNotReady());// Wait for I2C transmission to complete

    byte = (LO_NIBBLE(data) | LCD_BL | LCD_RS) & ~LCD_EN;// Construct data byte for I2C transmission
    I2cTransmit(0x27, byte);// Transmit data byte via I2C
    while(I2cNotReady());// Wait for I2C transmission to complete

}
void LcdWriteString(char *s)
{ // Write string to LCD character by character
    while (*s != '\0')
        LcdWriteChar(*s++);
}
void LcdSetPosition(unsigned char row, unsigned char column)
{
    switch(row)
    {
        case 1: 
            LcdWriteCommand(LCD_LINE1 + (column - 1), 1); 
            break;
        case 2:
            LcdWriteCommand(LCD_LINE2 + (column - 1), 1);
            break;
        case 3:
            LcdWriteCommand(LCD_LINE3 + (column - 1), 1);
            break;
        case 4:
            LcdWriteCommand(LCD_LINE4 + (column - 1), 1);
            break;
        default:
            LcdWriteCommand(LCD_LINE1 + (column - 1), 1);
    }
}
