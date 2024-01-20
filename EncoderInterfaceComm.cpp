#include "Arduino.h"
#include <EncoderInterfaceComm.h>

EncoderInterfaceComm::EncoderInterfaceComm(HardwareSerial &port, double cycle_time_ms, int n_pulses, double gear_ratio, double wheel_radius)
    : port(port), cycle_time(cycle_time_ms / 1000), done(false) {

    double counts_per_rotation = (n_pulses * gear_ratio * 4.0); // assumes 4X counting
    rpm_conversion_factor = 60.0 / counts_per_rotation;  
    speed_conversion_factor = 2 * pi * wheel_radius / counts_per_rotation;

    resetCount();
}

void EncoderInterfaceComm::resetCount() {
    prev_count = 0;
}

double EncoderInterfaceComm::getCountsPerSecond() {
    requestCount();
    receiveCount();

    return (count - prev_count) / cycle_time;
}

double EncoderInterfaceComm::getRpm() {
    return getCountsPerSecond() * rpm_conversion_factor;
}

double EncoderInterfaceComm::getSpeed() {
    // To do: get rpm and speed while only calling for the count once

    return getCountsPerSecond() * speed_conversion_factor;
}

void EncoderInterfaceComm::requestCount() {
    port.write('0');
}

void EncoderInterfaceComm::receiveCount() {
    byte i = 0;
    char read_char;
    char received_chars[num_chars];
 
    while (port.available() > 0 && done == false) {
        read_char = port.read();

        if (read_char != end_char) {
            received_chars[i] = read_char;

            i++;

            if (i >= num_chars) {
                i = num_chars - 1;
            }
        }
        else {
            received_chars[i] = '\0';

            i = 0;
            done = true;
        }
    }

    if (done == true) {
        count = 0;
        count = atol(received_chars);

        prev_count = count;
        done = false;
    }
}
