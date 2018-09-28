#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#define EXAMPLE 0

DynamixelShield dxl;
SoftwareSerial uart(7,8); //RX,TX

char buf[10] ;
char buf_index =0;
  
void setup() {
  // put your setup code here, to run once:
  dxl.begin(1000000, DXL_PACKET_VER_2_0);
  uart.begin(9600);
  delay(1000);
  dxl.ping();
  dxl.torqueOn(DXL_ALL_ID);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //bufClear();
  buf_index =0;
  while(uart.available())
  {
    buf[buf_index] = uart.read();
    buf_index++;
  }

  int32_t angle = (int32_t)atoi(buf);
  if( angle > 1800) angle = 1800;
  if( angle < -1800) angle = -1800;

  dxl.setGoalAngle(1, angle);
  uart.println(angle);
  uart.println(dxl.getGoalAngle(1));
  delay(500);



//
//if (dxl.getDxlCount() > 0)
//  {
//    dxl.setGoalAngle(1, 0);
//    uart.println(dxl.getGoalAngle(1));
//    delay(1000);
//    dxl.setGoalAngle(1, 900);
//    uart.println(dxl.getGoalAngle(1));
//    delay(1000);
//    dxl.setGoalAngle(1, 0);
//    uart.println(dxl.getGoalAngle(1));
//    delay(1000);
//    dxl.setGoalAngle(1, -900);
//    uart.println(dxl.getGoalAngle(1));
//    delay(1000);    
//  }

}

void bufClear()
{
  for(int a=0;a<10;a++)
  {
    buf[a] = NULL;
  }
  buf_index = 0;
}

