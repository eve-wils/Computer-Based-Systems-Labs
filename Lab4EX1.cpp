// Evelyn Wilson and Gavin McKee
// Lab 4, Exercise 1
// g++ -std=c++14 Lab4EX1.cpp -o Lab4EX1 -lwiringPi
// Reference: http://wiringpi.com/reference/i2c-library/


#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <iostream>

using namespace std;

/* Complete the code: I2c address of LCD, some LCD i2c address might be 0x27 */
int LCDAddr = 0x27; 

int BLEN = 1; // 1--open backlight.0--close backlight

int fd; // Linux file descriptor

// Send an  16 bits data to LCD buffer
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;

    /* Complete the code: write into the device using wiringPiI2CWrite */
    wiringPiI2CWrite(fd, temp); //used file descriptor and data as input for write

}

// Send control command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Send character to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Initialize the lcd
void init(){
    send_command(0x33); // Initialize 
    delay(5);
    send_command(0x32); // Initialize 
    delay(5);

    /* Complete the code: 4bit mode 2 Lines & 5*8 dots */
    send_command(0x28); //101000 in binary
    delay(5);

    send_command(0xD); // Enable display without cursor, 1101 in binary
    delay(5);

    /* Complete the code: Clear screen */
    send_command(0x01); 
    
}

// Print the message on the lcd
// x-> column, y-> line
void write(int x, int y, const char data[]){
    int addr, i;
    int tmp;
    if (x < 0) x = 0; if (x > 15) x = 15;
    if (y < 0) y = 0; if (y > 1)  y = 1;
    
    /* Complete the code: Target address, Move cursor */
    addr = 0x80 + 0x40 * y + x; //sets an address for the given x and y

    // Set the address
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

void print_info()
{
    printf("\n");
    printf("|***************************|\n");
    printf("|    IIC 1602 LCD test      |\n");
    printf("| --------------------------|\n");
    printf("| | LCD |            | Pi   |\n");
    printf("| --------------------------|\n");
    printf("| | GND | connect to | GND  |\n");
    printf("| | VCC | connect to | 5V   |\n");
    printf("| | SDA | connect to | SDA.1|\n");
    printf("| | SCL | connect to | SCL.1|\n");
    printf("| --------------------------|\n");
    printf("|                   ECEN3213|\n");
    printf("|***************************|\n");
    printf("Program is running...\n");
    printf("Press Ctrl+C to end the program\n");
}

int main(){

    // Set up WiringPi
    if (wiringPiSetup () == -1) exit (1);   

    // Init I2C, assign a buffer handler to variable fd
    fd = wiringPiI2CSetup(LCDAddr);

    /* Complete the code: in the init function*/
    init();

    print_info();

    /* Complete the code here to print on the LCD screen “Hello world!” and “I am xxx” in two lines.
     (xxx is your first name) */
    
    write(0, 0, "Hello World!"); //starts the message at line 0 position 0
    write(4, 1, "I am Eve!"); //starts the message at line 1, position 4

    /*
    Tips:
    1) Fill the code in function init() , write(), write_word()
    2) Use write() to display the messages
    */


    return 0;
}