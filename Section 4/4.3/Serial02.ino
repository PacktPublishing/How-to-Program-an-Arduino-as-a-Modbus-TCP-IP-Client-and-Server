#include <SPI.h>

int tanklevel;
String tmpstr;

void setup() {
  Serial.begin(9600);
  
  tanklevel = 12;

  tmpstr = "Tank Level: " + String(tanklevel) + " ft";
  Serial.println(tmpstr);

}

void loop() {
  

}
