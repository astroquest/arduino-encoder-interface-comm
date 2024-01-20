#ifndef ENCODERINTERFACECOMM_H
#define ENCODERINTERFACECOMM_H

#include "Arduino.h"

class EncoderInterfaceComm {
    public:
        EncoderInterfaceComm(HardwareSerial &port, double cycle_time_ms, int n_pulses, double gear_ratio, double wheel_radius);

        double getCountsPerSecond();
        double getRpm();
        double getSpeed();

        void resetCount();

    private:
        void requestCount();
        void receiveCount();

        HardwareSerial &port;
        double cycle_time;

        long prev_count;
        long count;

        static constexpr double pi = 3.14159265359;
        static constexpr byte num_chars = 32;
        static constexpr char end_char = '>';

        bool done;

        double rpm_conversion_factor;
        double speed_conversion_factor;
};

#endif