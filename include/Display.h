#pragma once
#include <Arduino.h>
#include <SerialGraphicLCD.h>

class Display
{

    public:
        Display();
        ~Display() = default;
        void init();

        void clear() { lcd.clearScreen(); }
        void outsideTemp( float temp );
        void insideTemp( float temp );

        void pressure( float value );
        void humidity( float value );
        void rssi( float value );

    private:
        LCD lcd;

        int printVal( int y, const char * str, float val );

        static const int GAP = 12;
        static const int DISPLAY_TOP = 63;
        static const int OUTSIDE_Y = DISPLAY_TOP;
        static const int INSIDE_Y = OUTSIDE_Y - GAP;
        static const int PRESSURE_Y = INSIDE_Y - GAP;
        static const int HUMIDITY_Y = PRESSURE_Y - GAP;
        static const int RSSI_Y = HUMIDITY_Y - GAP;

        static const int XCHARS = 128/6;
};

