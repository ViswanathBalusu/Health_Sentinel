//#include <stdio.h>
//#include "i2c-lcd.h"
//#include <sys/ioctl.h>
//
//int fd;
//
//int row=0;
//int col=0;
//
//int main() {
//    fd = open(I2C_DEVICE_FILE_PATH,O_RDWR);
//
//    if ( fd < 0) {
//        perror("Failed to open I2C device file.\n");
//        return -1;
//    }
//
//    /*set the I2C slave address  */
//    if (ioctl(fd,I2C_SLAVE,SLAVE_ADDRESS_LCD) < 0) {
//        perror("Failed to set I2C slave address.\n");
//        close(fd);
//        return -1;
//    }
//
//    lcd1602_init();
//    lcd1602_setCursorPosition(0, 0);
//    lcd1602_sendString("Beagle Bone");
//    sleep(1);
//    lcd1602_setCursorPosition(1, 0);
//    lcd1602_sendString("Hello World!");
//
//    return 0;
//}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

//#define I2C_ADDR 0x3F // I2C device address
#define I2C_ADDR 0x27
#define LCD_WIDTH 20 // Maximum characters per line
//#define LCD_WIDTH 16

// device constants
#define LCD_CHR 1 // Mode - Will Send Data
#define LCD_CMD 0 // Mode - Will Send Commands

#define LCD_LINE_1 0x80 // LCD RAM address for the 1st line
#define LCD_LINE_2 0xC0 // LCD RAM address for the 2nd line
#define LCD_LINE_3 0x94 // LCD RAM address for the 3rd line
#define LCD_LINE_4 0xD4 // LCD RAM address for the 4th line

#define LCD_BACKLIGHT 0x08 // On
//#define LCD_BACKLIGHT 0x00 // Off

#define ENABLE 0b00000100 // Enable bit

// Timing constants
#define E_PULSE 0.0005
#define E_DELAY 0.0005

int fd; // i2c file descriptor

void lcd_byte(int bits, int mode) {
    // Send byte to data pins
    // bits = the data
    // mode = 1 for data
    // 0 for command

    int bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    int bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    // High bits
    char buf[2];
    buf[0] = bits_high;
    buf[1] = (bits_high | ENABLE);
    write(fd, buf, 2);
    usleep(E_PULSE * 1000000);
    buf[1] = (bits_high & ~ENABLE);
    write(fd, buf + 1, 1);
    usleep(E_DELAY * 1000000);

    // Low bits
    buf[0] = bits_low;
    buf[1] = (bits_low | ENABLE);
    write(fd, buf, 2);
    usleep(E_PULSE * 1000000);
    buf[1] = (bits_low & ~ENABLE);
    write(fd, buf + 1, 1);
    usleep(E_DELAY * 1000000);
}

void lcd_toggle_enable(int bits) {
    // Toggle enable
    char buf[2];
    buf[0] = bits | ENABLE;
    buf[1] = bits & ~ENABLE;
    write(fd, buf, 2);
    usleep(E_DELAY * 1000000);
}

//void lcd_string(char *message, int line) {
//    // Send string to display
//    message = strncat(message, "                    ", LCD_WIDTH);
//    lcd_byte(line, LCD_CMD);
//    for (int i = 0; i < LCD_WIDTH; i++) {
//        lcd_byte((int)message[i], LCD_CHR);
//    }
//}

void lcd_string(char *message, int line) {
    // Send string to display
    char padded_message[LCD_WIDTH + 1];
    snprintf(padded_message, LCD_WIDTH + 1, "%-*.*s", LCD_WIDTH, LCD_WIDTH, message);
    lcd_byte(line, LCD_CMD);
    for (int i = 0; i < LCD_WIDTH; i++) {
        lcd_byte((int)padded_message[i], LCD_CHR);
    }
}


void lcd_init() {
    // Initialise display
    char *i2c_filename = "/dev/i2c-2"; // Change to the appropriate I2C bus
    if ((fd = open(i2c_filename, O_RDWR)) < 0) {
        printf("Failed to open the i2c bus device file: %s\n", i2c_filename);
        exit(1);
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("Failed to select the i2c slave address: %d\n", I2C_ADDR);
        close(fd);
        exit(1);
    }
    lcd_byte(0x33, LCD_CMD); // 110011 Initialise
    lcd_byte(0x32, LCD_CMD); // 110010 Initialise
    lcd_byte(0x06, LCD_CMD); // 000110 Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // 001100 Display On,Cursor Off, Blink Off
    lcd_byte(0x28, LCD_CMD); // 101000 Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD); // 000001 Clear display
    usleep(E_DELAY * 1000000);
}

int main() {
    // Main program block
    lcd_init();
    while (1) {
        // Send some test
        lcd_string("Akhil is Asshole", LCD_LINE_1);
        lcd_string("I2C LCD        <", LCD_LINE_2);
        usleep(3000000);

        // Send some more text
        lcd_string("Akhil is Asshole", LCD_LINE_1);
        lcd_string(">        I2C LCD", LCD_LINE_2);
        usleep(3000000);
    }
    return 0;
}