#include <MgsModbus.h>
#include <Ethernet.h>
#include <SPI.h>

int tanklevel;
int tanklevel_old;
int vfdspeed;
int vfdspeed_old;
bool valvepos;
bool valvepos_old;
bool pump;
bool pump_old;

MgsModbus Mb;

int acc = 0; // accumulator, seconds counter

// Ethernet settings (depending on MAC and Local network)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };
IPAddress ip(192, 168, 1, 120);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  
  Serial.begin(9600);

  // initialize ethernet shield
  Ethernet.begin(mac, ip, gateway, subnet);

  // server address
  Mb.remServerIP[0] = 192;
  Mb.remServerIP[1] = 168;
  Mb.remServerIP[2] = 1;
  Mb.remServerIP[3] = 121;
 
  tanklevel = 12;
  tanklevel_old = 12;
  vfdspeed = 150;
  vfdspeed_old = 150;
  valvepos = true;
  valvepos_old = true;
  pump = false;
  pump_old = false;

  DisplayCurrentValues();

}

void loop() {

  Poller();
  Mb.MbmRun();

  // send updated values
  CheckForDataChange();

}

void Poller() {

  delay(1);
  acc = acc + 1;

  // read input register - tanklevel
  if (acc == 3000) {
    Mb.Req(MB_FC_READ_INPUT_REGISTER,      100,1,0);
  }
  if (acc == 3200) {
    tanklevel = Mb.MbData[0];    
  }

  // read holding register - vfdspeed
  if (acc == 4000) {
    Mb.MbData[0] = 0;
    Mb.Req(MB_FC_READ_REGISTERS,      105,1,0);
  }   
  if (acc == 4200) {
    vfdspeed = Mb.MbData[0];
  }

  // read input status - valvepos
  if (acc == 5000) {
    Mb.MbData[0] = 0;
    Mb.Req(MB_FC_READ_DISCRETE_INPUT,      110,1,0);
  }   
  if (acc == 5200) {
    if (Mb.MbData[0] == 1) {
      valvepos = true;
    }
    else {
      valvepos = false;
    }
  }

  // read coil - pump
  if (acc == 6000) {
    Mb.MbData[0] = 0;
    Mb.Req(MB_FC_READ_COILS,      115,1,0);
  }   
  if (acc == 6200) {
    if (Mb.MbData[0] == 1) {
      pump = true;
    }
    else {
      pump = false;
    }
    acc = 0;
  }
  
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

  if (pump_old != pump) {
    data_has_changed = true;
    pump_old = pump;
  }

  if (data_has_changed == true) {
    DisplayCurrentValues();
  }
  
}

void DisplayCurrentValues() {

  String tmpstr;
  String pos;
  String pstat;

  if (valvepos == true) {
    pos = "Closed";
  }
  else {
    pos = "Open";
  }

  if (pump == true) {
    pstat = "Run";
  }
  else {
    pstat = "Stop";
  }

  tmpstr = "Tank Level: " + String(tanklevel) + " ft";
  tmpstr = tmpstr + " | VFD Speed: " + vfdspeed + " rpm";
  tmpstr = tmpstr + " | Valve: " + pos;
  tmpstr = tmpstr + " | Pump: " + pstat;
  Serial.println(tmpstr);
    
}

