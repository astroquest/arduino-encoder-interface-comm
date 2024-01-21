#include "Arduino.h"
#include <EncoderInterfaceComm.h>

EncoderInterfaceComm::EncoderInterfaceComm(HardwareSerial &port, double cycle_time_ms, int n_pulses, 
    double gear_ratio, double wheel_radius)
    : port(port), cycle_time(cycle_time_ms / 1000), done(false) {

    double counts_per_rotation = (n_pulses * gear_ratio * 4.0); // assumes 4X counting
    rpm_conversion_factor = 60.0 / counts_per_rotation;  
    speed_conversion_factor = 2 * pi * wheel_radius / counts_per_rotation;

    resetCounts();
}

void EncoderInterfaceComm::resetCounts() {
    for (unsigned int i = 0; i < n_motors; i++) {
        prev_counts[i] = 0;
    }
}

void EncoderInterfaceComm::update() {
    requestCounts();
    receiveCounts();

    for (unsigned int i = 0; i < n_motors; i++) {
        cpss[i] = (counts[i] - prev_counts[0]) / cycle_time;
        rpms[i] = cpss[i] * rpm_conversion_factor;
        speeds[i] = cpss[i] * speed_conversion_factor;
    }
}

void EncoderInterfaceComm::requestCounts() {
    port.write('0');
}

// based on https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
void EncoderInterfaceComm::receiveCounts() {
    byte i = 0;
    char read_char;
    char received_chars[num_chars];
    bool in_progress = false;
 
    while (port.available() > 0 && done == false) {
        read_char = port.read();

        if (in_progress == true) {
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
                in_progress == false;
                done = true;
            }
        }
        else if (read_char == start_char) {
            in_progress == true;
        }
    }

    if (done == true) {
        char temp[num_chars];
        strcpy(temp, received_chars);
        parseMessage(temp);

        for (unsigned int i = 0; i < n_motors; i++) {
            prev_counts[i] = counts[i];
        }

        done = false;
    }
}

void EncoderInterfaceComm::parseMessage(char *ptr_message) {
    char *ptr_token;

    ptr_token = strtok(*ptr_message, ",");
    counts[0] = atol(ptr_token);

    for (unsigned int i = 1; i < n_motors; i++) {

        ptr_token = strtok(NULL, ",");
        counts[i] = atol(ptr_token);
    }
}
