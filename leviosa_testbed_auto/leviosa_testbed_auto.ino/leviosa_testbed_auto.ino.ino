#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#define EXAMPLE 0
#define BUF_SIZE 20

#define INIT_ANGLE 0
#define END_ANGLE  1
#define RESOLUTION 2
#define ITER_CNT   3

DynamixelShield dxl;
SoftwareSerial uart(7,8); //RX,TX for give user input & monitoring percentange

char     buf[10] ;
char     serialData[20]; 
int32_t  command_set[4];
char     buf_index =0;

bool     working_state = false;
bool     start_state = false;
int32_t  comma_cnt = 0;
int32_t  angle = 0;
int32_t  step = 0; 
int8_t   index = 0;


int32_t parseCommand(char*, int32_t*) ;

  
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
  if(!working_state)
  {
    memset(command_set, 0, sizeof(command_set)); 
    waitUntilArrive(); 
    serialGetBuffer(serialData);

    // uart.readBytesUntil('\0', serialData, 20); 
    comma_cnt = parseCommand(serialData, command_set); 
    
    // always echo 
    //uart.print(serialData);
    // end of message is maked with a \n 
        
    // clear serialData array 
    memset(serialData, 0, sizeof(serialData)); 
  
    working_state = true;
    angle         = command_set[INIT_ANGLE];
    step          = (command_set[END_ANGLE] - command_set[INIT_ANGLE])/command_set[RESOLUTION];

    uart.println(command_set[INIT_ANGLE]);
    uart.println(command_set[END_ANGLE]);
    uart.println(command_set[RESOLUTION]);
  }
  
  else if(working_state)
  {
    if(angle == command_set[END_ANGLE]) working_state = false;
    
    dxl.setGoalAngle(1, angle);
    
    uart.println(angle);
    // uart.println(dxl.getGoalAngle(1));
    printProcessingPercentage(angle);
    
    angle += step;
  
    delay(10);
  }
}




  // int32_t angle = (int32_t)atoi(buf);
  // if( angle > 1800) angle = 1800;
  // if( angle < -1800) angle = -1800;

  // dxl.setGoalAngle(1, angle);
  // uart.println(angle);
  // uart.println(dxl.getGoalAngle(1));
  // delay(500);



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



// void bufClear()
// {
//   for(int a=0;a<10;a++)
//   {
//     buf[a] = NULL;
//   }
//   buf_index = 0;
// }

// 문자열을  정수로 변환
int atoi(String & str)
{
  char buf[BUF_SIZE];    // 문자열을 실수로 바꿀 때 필요한 buffer 정의
  str.toCharArray(buf, BUF_SIZE);
  return atoi(buf);    // 문자 배열을 정수로 변환
}

int32_t parseCommand(char* command, int32_t* return_values) 
{ 
  // parsing state machine 
  int i = 0, j = 0;
  int head_cnt = 0, tail_cnt = 0;
  int32_t cnt_number = 0; 
  char temp_command[] = "";
  boolean parsing_state = true;
  
  uart.print("head start ");
  while(parsing_state)
  {
    if(*(command + i) == 'A')
    {
      head_cnt++;
      i++;
    }
    else
    {
      parsing_state = false;
      working_state = false;
      return 0;
    }
    uart.println(head_cnt);
    if(head_cnt > 1 ) break;
  }

  uart.print("parsing start ");
  
  while(parsing_state) 
  { 
    //uart.println(cnt_number);
    switch(*(command + i)) 
    { 
      case ',':
        return_values[cnt_number] = (int32_t)atoi(temp_command);
        // uart.print('\0');
        // uart.println(return_values[cnt_number]);
        memset(temp_command, 0, sizeof(temp_command)); 
        cnt_number++;
        j = 0;
        break; 
     
      case 'B':
        tail_cnt++;
        if(tail_cnt >1)
        {
          parsing_state = false;
          working_state = false;
        }
        else
        {
          *(temp_command + j) = *(command + i);
          j++;
        }
        break;

      default:          
        *(temp_command + j) = *(command + i);
        j++;
        break; 
    } 
    i++; 
  }
  return_values[cnt_number] = (int32_t)atoi(temp_command);
 
  return cnt_number;
} 

void printProcessingPercentage(int32_t curr_angle)
{
  int32_t percentage = 100 * curr_angle / (command_set[END_ANGLE] - command_set[INIT_ANGLE]);
  uart.print("process : ");
  uart.println(percentage);
}

void  waitUntilArrive(void)
{
  while(1)
  {
    if(uart.available()) break;
  }
}

void serialGetBuffer(char* buffer)
{
  uint8_t index = 0;
  int32_t temp;

  temp = uart.read();
  while(temp != -1)
  { 
    uart.println(temp);
    
    *(buffer + index) = temp;
    index++;
    temp = uart.read();      
  }
}