#include <ModbusIP.h>
#include <Modbus.h>
#include <Ethernet.h>
#include <SPI.h>

int tanklevel;
int tanklevel_old;

const int tanklevel_ir = 100;

//Modbus IP Object
ModbusIP mb;

void setup() {
  Serial.begin(9600);

  // The media access control (ethernet hardware) address for the shield
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
  // The IP address for the shield
  byte ip[] = { 192, 168, 1, 120 };   
  //Config Modbus IP 
  mb.config(mac, ip);

  mb.addIreg(tanklevel_ir);
  
  tanklevel = 12;
  tanklevel_old = 12;

  mb.Ireg (tanklevel_ir, tanklevel);

  DisplayCurrentValues();

}

void loop() {

  mb.task();

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

