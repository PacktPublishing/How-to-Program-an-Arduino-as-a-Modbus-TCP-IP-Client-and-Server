#include <SPI.h>

int tanklevel;
int tanklevel_old;

void setup() {
  Serial.begin(9600);
  
  tanklevel = 12;
  tanklevel_old = 12;

  DisplayCurrentValues();

}

void loop() {

  // send updated values
  CheckForDataChange();

}

void CheckForDataChange() {

  boolean data_has_changed = false;

  if (tanklevel_old != tanklevel) {
    data_has_changed = true;
    tanklevel_old = tanklevel;
  }

  if (data_has_changed == true) {
    DisplayCurrentValues();
  }
  
}

void DisplayCurrentValues() {

  String tmpstr;

  tmpstr = "Tank Level: " + String(tanklevel) + " ft";
  Serial.println(tmpstr);
    
}

