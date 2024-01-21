#ifndef ENCODERINTERFACECOMM_H
#define ENCODERINTERFACECOMM_H

#include "Arduino.h"

static constexpr double pi = 3.14159265359;
static constexpr int n_motors = 2;

class EncoderInterfaceComm {
    public:
        EncoderInterfaceComm(HardwareSerial &port, double cycle_time_ms, int n_pulses, double gear_ratio, double wheel_radius);

        void resetCounts();
        void update();

        double cpss[n_motors]; // counts per second
        double rpms[n_motors];
        double speeds[n_motors];

    private:
        void requestCounts();
        void receiveCounts();
        void parseMessage(char*);

        HardwareSerial &port;
        double cycle_time;

        long counts[n_motors];
        long prev_counts[n_motors];

        static constexpr byte num_chars = 32;
        static constexpr char start_char = '<';
        static constexpr char end_char = '>';

        bool done;

        double rpm_conversion_factor;
        double speed_conversion_factor;
};

#endif