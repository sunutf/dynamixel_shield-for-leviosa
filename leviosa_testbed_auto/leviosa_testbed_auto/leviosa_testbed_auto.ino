#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#define EXAMPLE 0
#define BUF_SIZE 20

#define INIT_ANGLE 0
#define END_ANGLE  1
#define RESOLUTION 2
#define ITER_CNT   3

//strongly recommand to use, these values, except ITER_CNT_VAL
const int INIT_ANGLE_VAL = 0;
const int END_ANGLE_VAL = 1800;
const int RESOLUTION_VAL = 180;
const int ITER_CNT_VAL = 10;

DynamixelShield dxl;
SoftwareSerial uart(12,13); //RX,TX for give user input & monitoring percentange
//SoftwareSerial SoftSerial(7,8);

#define Debug Serial
char     buf[10] ;
char     serialData[20]; 
char     command_set[4];
char     buf_index =0;
char     command_packet[9] = {'#',0,0,0,0,0,0,0,'@'}; //
char     ack_packet[4] = {'#', 0, 0,'@'};

bool     hearing_state = false;
bool     start_state = false;
int32_t  comma_cnt = 0;
int32_t  angle = 0;
int32_t  step_val = 0; 
int8_t   index = 0;
int32_t  cnt = 0;
int8_t   rx_cnt =0;

unsigned long prev_time;

bool retry = false;

uint32_t parseCommand(char*) ;

  
void setup() {
  // put your setup code here, to run once:
    
  dxl.begin(1000000, DXL_PACKET_VER_2_0);
  delay(1000);
  dxl.ping();
  dxl.torqueOn(DXL_ALL_ID);
  // dxl.torqueOff(DXL_ALL_ID);
//  Debug.begin(9600);
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
    step_val      = (END_ANGLE_VAL - INIT_ANGLE_VAL)/RESOLUTION_VAL;
    dxl.setGoalAngle(1, angle);
  }

  //read packet from processing
  else 
  {
    prev_time = millis();
    if(uart.available() >0)
    {
      
      char c = uart.read();
//       Debug.print(" =");
//       Debug.print((int)c);
//     

      if(!start_state)
      {
        if(c=='#') start_state = true;
      }
      else
      { 
        if((millis()-prev_time)>2000) retry = true; 
        if(c=='@')
        {
          if(rx_cnt == 2)
          {
            retry = false;
            prev_time = millis();
  //           Debug.print("WE SET : ");
  //           Debug.println(cnt);
            if(cnt == parseCommand(command_set))
            {
               
               dxl.setGoalAngle(1, angle);
//               Debug.println(cnt);
               
               ack_packet[1] = (cnt >> 8 | 0);
               ack_packet[2] = (cnt | 0) ;
               packaging(ack_packet); 
               uart.write((uint8_t *)ack_packet, 4); // #, cnt_H, cnt_L, @
               
               angle += step_val;
               cnt++; 
               
               if(cnt == RESOLUTION_VAL) while(1);
            }
            else{
  //            Debug.println(cnt);
//               Debug.print("RE");
//              
//               Debug.println(cnt);
               
               ack_packet[1] = ((cnt-1) >> 8 | 0);
               ack_packet[2] = ((cnt-1) | 0) ;
               packaging(ack_packet);
               delay(100);
               uart.write((uint8_t *)ack_packet, 4); // #, cnt_H, cnt_L, @
               
            }
            delay(100);
            memset(command_set, 0, sizeof(command_set)); 
            memset(ack_packet, 0, sizeof(ack_packet)); 
           
            start_state = false;
            rx_cnt = 0;     
          }
           else
          {
            command_set[rx_cnt++] = c;
           }    
        }
        else
        {
          command_set[rx_cnt++] = c;
        }
        
        if(retry)
        {
//          Debug.print("RE1");
//            
//         Debug.println(cnt);
//         
         ack_packet[1] = ((cnt-1) >> 8 | 0);
         ack_packet[2] = ((cnt-1) | 0) ;
         packaging(ack_packet);
         uart.write((uint8_t *)ack_packet, 4); // #, cnt_H, cnt_L, @
         delay(100);
         memset(command_set, 0, sizeof(command_set)); 
         memset(ack_packet, 0, sizeof(ack_packet)); 
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

void packaging(char* buf)
{
  buf[0] = '#';
  buf[3] = '@';
}

uint32_t parseCommand(char* buf)
{
  uint32_t l_cnt = ((uint8_t)(buf[0]) << 8) | (uint8_t)buf[1];
//  Debug.print("GET : ");
//  Debug.println(l_cnt);
  return l_cnt;
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

 
