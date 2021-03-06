#include <ModbusIP.h>
#include <Modbus.h>
#include <Ethernet.h>
#include <SPI.h>

int tanklevel;
int tanklevel_old;
int vfdspeed;
int vfdspeed_old;
bool valvepos;
bool valvepos_old;

const int tanklevel_ir = 100;
const int vfdspeed_hr = 105;
const int valvepos_is = 110;

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
  mb.addHreg(vfdspeed_hr);
  mb.addIsts(valvepos_is);
  
  tanklevel = 12;
  tanklevel_old = 12;
  vfdspeed = 150;
  vfdspeed_old = 150;
  valvepos = true;
  valvepos_old = true;

  mb.Ireg (tanklevel_ir, tanklevel);
  mb.Hreg (vfdspeed_hr, vfdspeed);
  mb.Ists (valvepos_is, valvepos);

  DisplayCurrentValues();

}

void loop() {

  mb.task();

  vfdspeed = mb.Hreg (vfdspeed_hr);

  // send updated values
  CheckForDataChange();

}

void CheckForDataChange() {

  boolean data_has_changed = false;

  if (tanklevel_old != tanklevel) {
    data_has_changed = true;
    tanklevel_old = tanklevel;
  }

  if (vfdspeed_old != vfdspeed) {
    data_has_changed = true;
    vfdspeed_old = vfdspeed;
  }

  if (valvepos_old != valvepos) {
    data_has_changed = true;
    valvepos_old = valvepos;
  }

  if (data_has_changed == true) {
    DisplayCurrentValues();
  }
  
}

void DisplayCurrentValues() {

  String tmpstr;
  String pos;

  if (valvepos == true) {
    pos = "Closed";
  }
  else {
    pos = "Open";
  }

  tmpstr = "Tank Level: " + String(tanklevel) + " ft";
  tmpstr = tmpstr + " | VFD Speed: " + vfdspeed + " rpm";
  tmpstr = tmpstr + " | Valve: " + pos;
  Serial.println(tmpstr);
    
}

