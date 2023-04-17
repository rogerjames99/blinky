#include <Arduino.h>

#ifdef ARDUINO_TEENSY41
    static int LED = LED_BUILTIN;
#else
    #ifdef ARDUINO_NUCLEO_F103RB
        static int LED = D13;
    #else
        #pragma GCC error "Unknown board type"
        #include "/stophere" // I hope this does not exist!
    #endif
#endif

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
    digitalToggle(LED);
    delay(200);
}
