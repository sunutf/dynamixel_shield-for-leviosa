//for ESP3//

uint32_t init_angle  = 0;
uint32_t final_angle = 0;
uint32_t resolution  = 0;
uint8_t  iter_rx_cnt = 0;

uint32_t step  = 0;

uint32_t total_rx_cnt= 0;
uint32_t curr_rx_cnt = 0;
uint32_t curr_angle  = 0;
uint32_t rx_cnt      = 0;
uint32_t cnt = 0;

char ack_packet[4] = {'#', 0, 0, '@'};
char command_packet[7];

unsigned long  prev_time = 0;
bool ret          = true;
bool cali_start   = false;
bool parse_start  = false;
bool retry        = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0)
  {
     prev_time = millis();
    char c = Serial2.read();
    Serial.print(" ==");
    Serial.print((int)c);
//    
   

    if(!cali_start)
    {
      if(c == '@')
      {
        if(rx_cnt == 7)
        {
          parse_start = false;
          cali_start  = true;
          rx_cnt = 0;

          parseInitCommand(command_packet);
          Serial.println("Cali Init Done");
          Serial.println(init_angle);
          Serial.println(final_angle);
          Serial.println(resolution);
          Serial.println(iter_rx_cnt);

          ack_packet[1] = (cnt >> 8 | 0);
          ack_packet[2] = (cnt | 0);
          
          ret = Serial2.write((uint8_t *)ack_packet, 4);
          memset(command_packet, 0, sizeof(command_packet));
        }
      }

      if(parse_start)
      {
        command_packet[rx_cnt] = c;
        rx_cnt++;
      }
      if(c == '#') parse_start = true;
    }

    else
    {
    
     if(!parse_start)
     {
        if(c == '#')
        {
          parse_start = true;
          //Serial.println("Cali run\n");
        }
     }
     
     else
     {
      if((millis()-prev_time)>2000) retry = true; 
        if(c == '@')
        {
          if(rx_cnt == 2)
          {
             retry = false;
              prev_time = millis();
            if(curr_angle > final_angle)
            {
              Serial.printf("\n curr angle : %d ", curr_angle);
              Serial.print("FINAL, now calibration end");
              /// end
              while(1);
            }

//            Serial.print("WE SET");
//            Serial.println(cnt);
            if(cnt == parseCommand(command_packet)){
                  curr_angle += step;
               Serial.println(cnt);
              //calibrationLuxStore(id, curr_angle, iter_rx_cnt);
              cnt++;
              ack_packet[1] = (cnt >> 8 | 0);
              ack_packet[2] = (cnt | 0);
              packaging(ack_packet);
              ret = Serial2.write((uint8_t *)ack_packet, 4);
              Serial.write((uint8_t *)ack_packet, 4);
              
              
            }
            else{
          
               Serial.print("ERROR mis match current angle : ");
               Serial.println(cnt);
               ack_packet[1] = (cnt >> 8 | 0);
               ack_packet[2] = (cnt | 0);
               packaging(ack_packet);
               Serial2.write((uint8_t *)ack_packet, 4); // #, cnt_H, cnt_L, @
              
            }
            
            memset(command_packet, 0, sizeof(command_packet));
            memset(ack_packet, 0, sizeof(ack_packet));
        
            parse_start = false;
            rx_cnt=0;
          }
           else
          {
           command_packet[rx_cnt++] = c;
         }
        }
        
        else
        {
          command_packet[rx_cnt++] = c;
        }

        if(retry)
        {
         Serial.print("RE1");
         Serial.println(cnt);
         
         ack_packet[1] = ((cnt-1) >> 8 | 0);
         ack_packet[2] = ((cnt-1) | 0) ;
         packaging(ack_packet);
         Serial2.write((uint8_t *)ack_packet, 4); // #, cnt_H, cnt_L, @
         
         memset(command_packet, 0, sizeof(command_packet)); 
         memset(ack_packet, 0, sizeof(ack_packet)); 
        }
      }
    }
  }
}

void parseInitCommand(char* buf)
{
  init_angle  = ((buf[0]) << 8) | buf[1];
  final_angle = ((buf[2]) << 8) | buf[3];
  resolution  = ((buf[4]) << 8) | buf[5];
  iter_rx_cnt = buf[6];

  step = (final_angle - init_angle)/resolution;
}

void packaging(char* buf)
{
  buf[0] = '#';
  buf[3] = '@';
}

uint32_t parseCommand(char* buf)
{
  uint32_t angle = ((buf[0]) << 8) | buf[1];
//  Serial.print("GET : ");
//  Serial.println(angle);
  return angle;
}
