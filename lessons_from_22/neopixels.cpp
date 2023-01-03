#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "neopixels.h"
#include "mbedtls/md.h"

#define PIN        38
#define MAX_BRIGHTNESS 64

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void neopixels::init( void )
{
    pixels.begin();
    memset( all_pixels, 0, sizeof(all_pixels) );
    updated = false;

    for( uint8_t id = 0; id < NUMPIXELS; id++ )
    {
        pixels.setPixelColor( id, pixels.Color(0, 100, 0) );

        if( id > 0)
        pixels.setPixelColor( (id-1), pixels.Color(0, 0, 0) );

        pixels.show();
        delay(50);
    }
}

void neopixels::set_color( uint8_t id, uint8_t r, uint8_t g, uint8_t b )
{
    if( NUMPIXELS > id )
    {
        all_pixels[id].red = r;
        all_pixels[id].green = g;
        all_pixels[id].blue = b;
        updated = false;
    }
}

void neopixels::run( void )
{
    if( false == updated )
    {
        for( uint8_t id = 0; id < NUMPIXELS; id++ )
        {
            pixels.setPixelColor( id, pixels.Color(all_pixels[id].red, 
                                                   all_pixels[id].green, 
                                                   all_pixels[id].blue) );
        }

        pixels.show();
        updated = true;
    }
}

void neopixels::reset( void )
{
    memset( all_pixels, 0, sizeof(all_pixels) );
    updated = false;
}

void neopixels::generate_colors_from_message( char * message, uint32_t message_len )
{
    char hash_string_buffer[126] = {0};
    uint32_t hash_string_buffer_index = 0;
    byte color1_results[NUMPIXELS];
    byte color2_results[NUMPIXELS];
    
    reset();
    char color1_seed[] = "first_color_seed";
    char color2_seed[] = "second_color_seed";
    memset(hash_string_buffer, 0, sizeof(hash_string_buffer));
    memcpy(hash_string_buffer, message, message_len);
    hash_string_buffer_index += message_len;
    memcpy(&hash_string_buffer[message_len], color1_seed, sizeof(color1_seed));
    hash_string_buffer_index += sizeof(color1_seed);
    random_color_generator(hash_string_buffer, hash_string_buffer_index, color1_results, sizeof(color1_results));

    hash_string_buffer_index = 0;
    memset(hash_string_buffer, 0, sizeof(hash_string_buffer));
    memcpy(hash_string_buffer, message, message_len);
    hash_string_buffer_index += message_len;
    memcpy(&hash_string_buffer[message_len], color2_seed, sizeof(color2_seed));
    hash_string_buffer_index += sizeof(color1_seed);
    random_color_generator(hash_string_buffer, hash_string_buffer_index, color2_results, sizeof(color2_results));

    for (uint32_t i = 0; i < NUMPIXELS; i++)
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;

        uint8_t color1_to_update = random(3);
        uint8_t color2_to_update = random(3);

        generate_color_from_hash(color1_results, i, color1_to_update, &red, &green, &blue);
        generate_color_from_hash(color2_results, i, color2_to_update, &red, &green, &blue);
        
        set_color( i, red, green, blue );
    }
}

void neopixels::generate_color_from_hash( byte * color_buff_source, uint32_t index, uint8_t selected_color, uint8_t * r, uint8_t * g, uint8_t * b )
{
    switch (selected_color)
    {
        case 0:
            *r = (uint8_t)map(color_buff_source[index], 0, UINT8_MAX, 0, MAX_BRIGHTNESS);
            break;
        case 1:
            *g = (uint8_t)map(color_buff_source[index], 0, UINT8_MAX, 0, MAX_BRIGHTNESS);
            break;
        case 2:
            *b = (uint8_t)map(color_buff_source[index], 0, UINT8_MAX, 0, MAX_BRIGHTNESS);
            break;
        default:
            break;
    }
}

void neopixels::random_color_generator( char * payload, size_t payload_len, byte * result, size_t result_len )
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
    mbedtls_md_finish(&ctx, result);
    mbedtls_md_free(&ctx);
}


