#include <EncoderInterfaceComm.h>

double cycle_time = 1000;
int n_pulses = 11;
double gear_ratio = 56;
double wheel_radius = 0.3;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("Ready");
}

EncoderInterfaceComm eicomm(Serial, cycle_time, n_pulses, gear_ratio, wheel_radius);

void loop() {
  eicomm.update();
  double rpms[2] = {eicomm.rpms[0], eicomm.rpms[1]};
}
