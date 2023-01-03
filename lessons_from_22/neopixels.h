#ifndef _SEASONS_GREETINGS_NEOPIXELS_
#define _SEASONS_GREETINGS_NEOPIXELS_

#define NUMPIXELS  32 // Popular NeoPixel ring size
typedef struct 
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} neopixels_str;

class neopixels
{
    private:
        uint8_t pixel_count;
        bool updated;
        neopixels_str all_pixels[NUMPIXELS];
        void set_color( uint8_t id, uint8_t r, uint8_t g, uint8_t b );
        void random_color_generator(char *payload, size_t payload_len, byte *result, size_t result_len);
        void generate_color_from_hash(byte *color_buff_source, uint32_t index, uint8_t selected_color, uint8_t *r, uint8_t *g, uint8_t *b);

    public:
        void init( void );
        void run( void );
        void reset( void );
        void generate_colors_from_message(char *message, uint32_t message_len);
};

#endif