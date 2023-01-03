#ifndef _SEASONS_GREETINGS_DISPLAY_
#define _SEASONS_GREETINGS_DISPLAY_

#define DISPLAY_MESSAGE_LENGTH (126)
#define DISPLAY_NAME_LENGTH    (21)

class seasons_greetings_display
{
    private:
        char message_buffer[DISPLAY_MESSAGE_LENGTH]; 
        char name_buffer[DISPLAY_NAME_LENGTH]; 

        void clear( void );
        void display_string_formatter( char * message, uint32_t len );
        void display_message_and_name( char * message, uint32_t message_len, char * name, uint32_t name_len );
        uint8_t display_name_cursor( char * name, uint32_t len );
    public:
        void init( void );
        void update( char * message, uint32_t message_len, char * name, uint32_t name_len );
};

#endif