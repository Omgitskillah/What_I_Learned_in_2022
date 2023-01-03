#include <Arduino.h>
#include "onboard_led.h"

void OnboardLed::init( void )
{
    pinMode(LED_BUILTIN, OUTPUT);
    reset();
}

void OnboardLed::reset( void )
{
    previous_millis = millis();
    state = ONBOARD_LED_OFF;
    on_time = 0;
    off_time = 0;
}

void OnboardLed::run( void )
{
    uint32_t current_millis = millis();
    uint32_t hold_time = (state == ONBOARD_LED_OFF) ? off_time : on_time;

    if( (on_time != 0) && 
        ((current_millis - previous_millis) >= hold_time) )
    {
        // toggle
        (state == ONBOARD_LED_OFF) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
        // switch state
        state = (state == ONBOARD_LED_OFF) ? ONBOARD_LED_ON : ONBOARD_LED_OFF;
        // refresh millis
        previous_millis = millis();
    }
}

void OnboardLed::update( uint32_t on, uint32_t off )
{
    on_time = on;
    off_time = off;
}