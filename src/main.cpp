#include <mbed.h>
#include "lcd.h"


int main()
{

  BufferedSerial pc (USBTX, USBRX, 9600);


  init_lcd();
  clr_lcd();

//   print_lcd("  in main ...");
//  write_cmd(0xc0);   //set cursor to second line
//  wait_us(40);
//  print_lcd("      Testing");

  int count = 0;

  while(1)
  {
    char buffer[16];

    int len = sprintf( buffer, "Count: %d\n", count++ & 0xFF );
    pc.write( static_cast<const char*>(buffer), len );

    clr_lcd();
    print_lcd("  Hello wurldz");
    write_cmd(0xc0);   //set cursor to second line
    wait_us(40);
    print_lcd( buffer );


    ThisThread::sleep_for(1000ms);
  }

}