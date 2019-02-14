#include "EEPROM.h"
//소숫점 2자리까지 저장.
int addr = 0;

float offset_val[4][3][4] = {
  {{12.1234,0.5,0.6,0.4},{0.1,0.2,0.3,0.4},{0,0,0,0},},
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},},
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},},
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},}
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
  for(int quad = 0; quad<4; quad++){
   for(int height = 0; height<3; height++){
      for(int deg = 0; deg<4; deg++){
           addr +=4;
           Serial.println(EEPROM.readFloat(addr), 4);
       }
    }
  }
  addr = 0;
  Serial.println("===WRITE===");
  for(int quad = 0; quad<4; quad++){
    for(int height = 0; height<3; height++){
       for(int deg = 0; deg<4; deg++){
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
