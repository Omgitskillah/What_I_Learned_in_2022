#ifndef _SEASONS_GREETINGS_LED_
#define _SEASONS_GREETINGS_LED_

class OnboardLed
{
    typedef enum onboard_led_state 
    {
        ONBOARD_LED_OFF,
        ONBOARD_LED_ON
    } onboard_led_state_t;

    private:
        uint32_t on_time;
        uint32_t off_time;
        uint32_t previous_millis;
        onboard_led_state_t state;
        void reset(void);
    public:
        void init( void );
        void run( void );
        void update( uint32_t on_time, uint32_t off_time );
};

#endif