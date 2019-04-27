#include "EEPROM.h"
//소숫점 2자리까지 저장.
#define QUAD_CNT 4
#define HEIGHT_CNT 3
#define DEG_CNT 2
int addr = 0;

float offset_val[4][3][2] = {
  {{12.1234,0.5},{0.1,0.2},{0,0},},
  {{0,0},{0,0},{0,0},},
  {{0,0},{0,0},{0,0},},
  {{0,0},{0,0},{0,0},}
};


#define EEPROM_SIZE 128
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start...");
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  Serial.println("===READ===");
  for(int quad = 0; quad<QUAD_CNT; quad++){
   for(int height = 0; height<HEIGHT_CNT; height++){
      for(int deg = 0; deg<DEG_CNT; deg++){
           addr +=4;
           Serial.println(EEPROM.readFloat(addr), 4);
       }
    }
  }
  addr = 0;
  Serial.println("===WRITE===");
  for(int quad = 0; quad<QUAD_CNT; quad++){
    for(int height = 0; height<HEIGHT_CNT; height++){
       for(int deg = 0; deg<DEG_CNT; deg++){
           addr +=4;
           EEPROM.writeFloat(addr, offset_val[quad][height][deg]);
           Serial.print(offset_val[quad][height][deg],4); Serial.println(" ");
       }
    }
  }
  EEPROM.commit();
}

void loop() {
  // put your main code here, to run repeatedly:
  
 
}
