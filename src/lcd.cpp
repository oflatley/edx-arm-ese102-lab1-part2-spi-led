#include <mbed.h>
#include "lcd.h"

#define ENABLE 0x08         //Will be ORed in to data value to strobe E bit
#define COMMAND_MODE 0x00   //used to clear RS line to 0, for command transfer
#define DATA_MODE 0x04      //used to set RS line to 1, for data transf


  // used to copy shift reg to storage reg on shift IC
static  DigitalOut shiftReg_stcp(D10);


  // SPI api.
static SPI spi(D11,D12,D13);





void init_lcd(void) {   //follow designated procedure in data sheet

    spi.format(8,0);
    spi.frequency(1000000);


    shiftReg_stcp.write(1);

    spi.write(0x5555);
    shiftReg_stcp.write(0);
    shiftReg_stcp.write(1);



    thread_sleep_for (40);
    shift_out(0x30);    //function set 8-bit
    wait_us(37);
    write_cmd(0x20);    //function set
    wait_us(37);
    write_cmd(0x20);    //function set
    wait_us(37);
    write_cmd(0x0C);    //display ON/OFF
    wait_us(37);
    write_cmd(0x01);    //display clear
    wait_us(1520);
    write_cmd(0x06);    //entry-mode set
    wait_us(37);
    write_cmd(0x28);    //function set
    wait_us(37);
}

void write_4bit(int data, int mode) {  //mode is RS line, ie 0 for cmd, 1 for data
    int hi_n;
    int lo_n;
    hi_n = (data & 0xF0);           //form the two 4-bit nibbles that will be sent
    lo_n = ((data << 4) &0xF0);
    shift_out(hi_n | ENABLE | mode); //send each word twice, strobing the Enable line
    wait_us(1);
    shift_out(hi_n & ~ENABLE);
    shift_out(lo_n | ENABLE | mode);
    wait_us(1);
    shift_out(lo_n & ~ENABLE);
}

void shift_out(int data) {     //Invokes SPI

    shiftReg_stcp.write(0);

    // write to SR using mbed api
    spi.write(data);

    // SR: copy shift input pins to parallel output pins on rising edge ...
    shiftReg_stcp.write(1);

}

void write_cmd(int cmd) {
    write_4bit(cmd, COMMAND_MODE);
}

void write_data(char c) {
    write_4bit(c, DATA_MODE);    //1 for data mode
}

void clr_lcd(void) {
    write_cmd(0x01);    //display clear
    wait_us(1520);
}

void print_lcd(const char *string) {
    while(*string){
        write_data(*string++);
            wait_us(40);
    }
}
