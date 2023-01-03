#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "display.h"


Adafruit_SH1107 oled = Adafruit_SH1107(64, 128, &Wire);

#define NAME_Y_CURSOR_INDEX (56)
#define OLED_ADDRESS        (0x3C) // Address 0x3C default
#define CHAR_WIDTH          (6)
#define DISPLAY_HEIGH       (64)
#define DISPLAY_WIDTH       (128)
#define LINE_CHAR_COUNT     (21)

void seasons_greetings_display::init( void )
{
    oled = Adafruit_SH1107(DISPLAY_HEIGH, DISPLAY_WIDTH, &Wire);
    oled.begin(OLED_ADDRESS, true); 
    //Clear the buffer.
    oled.clearDisplay();
    oled.display();
    oled.setRotation(1);

    oled.setTextSize(1);
    oled.setTextColor(SH110X_WHITE);
    oled.setCursor(0,0);
    oled.println("Starting");
    oled.display();
}

void seasons_greetings_display::update( char * message, uint32_t message_len, char * name, uint32_t name_len )
{
    if( (DISPLAY_MESSAGE_LENGTH >= message_len) &&
        (DISPLAY_NAME_LENGTH >= name_len) )
    {
        memset( message_buffer, 0, DISPLAY_MESSAGE_LENGTH );
        memset( name_buffer, 0, DISPLAY_NAME_LENGTH );

        memcpy( message_buffer, message, message_len );
        memcpy( name_buffer, name, name_len );

        clear();

        display_message_and_name( message_buffer, DISPLAY_MESSAGE_LENGTH, name_buffer, DISPLAY_NAME_LENGTH );
    }
}


void seasons_greetings_display::display_message_and_name( char * message, uint32_t message_len, char * name, uint32_t name_len )
{
    oled.setTextSize(1);
    oled.setTextColor(SH110X_WHITE);
    oled.setCursor(0,0);

    display_string_formatter( message, message_len );

    oled.println(message);
    oled.display();

    uint8_t name_cursor = display_name_cursor( name, name_len );
    oled.setCursor( name_cursor, NAME_Y_CURSOR_INDEX );
    oled.print("- ");
    oled.println(name);
    oled.display();
}

void seasons_greetings_display::clear( void )
{
    oled.clearDisplay();
    oled.display();
}

void seasons_greetings_display::display_string_formatter( char * message, uint32_t len )
{
    uint32_t last_space_index = 0;
    uint32_t next_line_start = 0;
    uint32_t new_length = 0;
    uint32_t i = 0;

    if( LINE_CHAR_COUNT <= strlen(message) )
    {
        for( i; i <= LINE_CHAR_COUNT; i++ )
        {
            if( ' ' == message[i] )
            {
                last_space_index = i;
            }
        }

        if( len > LINE_CHAR_COUNT )
        {
            message[last_space_index] = '\n';

            next_line_start = last_space_index + 1;
            new_length = len - next_line_start;
            display_string_formatter( &message[next_line_start], new_length );
        }
    }
}

uint8_t seasons_greetings_display::display_name_cursor( char * name, uint32_t len )
{
    uint32_t actual_name_length = 0;

    for( uint32_t i = 0; i < len; i++ )
    {
        if( NULL == name[i] )
        {
            actual_name_length = i;
            break;
        }
    }
    // add space for the hyphen and space
    actual_name_length += 2; 

    return (uint8_t)((len - actual_name_length) * CHAR_WIDTH);
}

