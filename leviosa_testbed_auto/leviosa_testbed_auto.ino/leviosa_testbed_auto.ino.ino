#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#define EXAMPLE 0
#define BUF_SIZE 20

#define INIT_ANGLE 0
#define END_ANGLE  1
#define RESOLUTION 2
#define ITER_CNT   3

const int INIT_ANGLE_VAL = 0;
const int END_ANGLE_VAL = 1800;
const int RESOLUTION_VAL = 1800;
const int ITER_CNT_VAL = 10;

DynamixelShield dxl;
SoftwareSerial uart(7,8); //RX,TX for give user input & monitoring percentange

char     buf[10] ;
char     serialData[20]; 
char  command_set[4];
char     buf_index =0;
char     command_packet[9] = {'#',0,0,0,0,0,0,0,'@'}; //

bool     hearing_state = false;
bool     start_state = false;
int32_t  comma_cnt = 0;
int32_t  angle = 0;
int32_t  step = 0; 
int8_t   index = 0;
int32_t  cnt = 0;
int8_t  rx_cnt =0;


int32_t parseCommand(char*) ;

  
void setup() {
  // put your setup code here, to run once:
    
  dxl.begin(1000000, DXL_PACKET_VER_2_0);
  delay(1000);
  dxl.ping();
  dxl.torqueOn(DXL_ALL_ID);
  // dxl.torqueOff(DXL_ALL_ID);

  uart.begin(9600);

  
}

void loop() {
// put your main code here, to run repeatedly:
  //write packet to processing 
  if(!hearing_state)
  {
    //send packet with initial values
    command_packet[1] = (INIT_ANGLE_VAL >> 8 | 0);
    command_packet[2] = (INIT_ANGLE_VAL | 0);
    command_packet[3] = (END_ANGLE_VAL >> 8 | 0);
    command_packet[4] = (END_ANGLE_VAL | 0);
    command_packet[5] = (RESOLUTION_VAL >> 8 | 0);
    command_packet[6] = (RESOLUTION_VAL | 0);
    command_packet[7] = ITER_CNT_VAL;

    uart.write(command_packet, 9);
    
    hearing_state = true;
    angle         = INIT_ANGLE_VAL;
    step          = (END_ANGLE_VAL - INIT_ANGLE_VAL)/RESOLUTION_VAL;
    dxl.setGoalAngle(1, angle);
  }

  //read packet from processing
  else 
  {
    
   
    if(uart.available() >0)
    {
      char c = uart.read();

      if(!start_state)
      {
        if(c=='#') start_state = true;
      }
      else
      {
        if(c=='@')
        {
          if(cnt == parseCommand(command_set))
          {
             angle += step;
             dxl.setGoalAngle(1, angle);
             cnt++; 
             memset(command_set, 0, sizeof(command_set)); 
             delay(10);
          }
          start_state = false;
          rx_cnt = 0;
         
        }
        else
        {
          command_set[rx_cnt++] = c;
        }
      }
    }
  }
}
//    
//    waitUntilArrive();
//    
//    
//    uart.readBytesUntil('\0', serialData, 20);
//    comma_cnt = parseCommand(serialData, command_set); 
//    if(comma_cnt == 1 && command_set[0] == cnt)
//    {
//      cnt++;
//      angle += step;
//      dxl.setGoalAngle(1, angle); 
//      delay(10);
//      sendPacket(cnt);
//    }
//  } 
//}

int32_t parseCommand(char* buf)
{
  int32_t cnt = (buf[0]) << 8 + buf[1];
  return cnt;
}
//
//void printProcessingPercentage(int32_t curr_angle)
//{
//  int32_t percentage = 100 * curr_angle / (command_set[END_ANGLE] - command_set[INIT_ANGLE]);
//  uart.print("process : ");
//  uart.println(percentage);
//}
//
//void  waitUntilArrive(void)
//{
//  while(1)
//  {
//    if(uart.available()) break;
//  }
//}

 