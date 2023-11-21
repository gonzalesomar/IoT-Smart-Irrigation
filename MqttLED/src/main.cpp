#include <Arduino.h>
#include <XSpaceV2.h>

XSpaceV2Board board;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  board.SerialInfo(true);
  board.Wifi_init("redpucp","C9AA28BA93");
  board.Mqtt_init("www.xspace.pe",1883);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    if(!board.Mqtt_IsConnected()){
      board.Mqtt_Connect("XSpace", "Omar", 1234);
    }
  }
}