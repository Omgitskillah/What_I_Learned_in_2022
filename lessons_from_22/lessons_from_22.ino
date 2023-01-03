#include "lesson_manager.h"
#include "neopixels.h"
#include "display.h"

#define FETCH_DATA_PERIOD      60000 // 1 min

lesson_manager lessons;
neopixels pix;
seasons_greetings_display screen;
uint32_t previous_millis = 0;

bool show_lesson_and_name(char *lesson, uint32_t lesson_len, char *name, uint32_t name_len, int row);

void setup()
{
    Serial.begin(115200);
    delay(1000);
    // connect to WiFi
    pix.init();
    screen.init();
}

void loop()
{
    /**
     * @brief
     * 1. check if connected to wifi
     * 2. if connected,
     * 3. check if there is any lesson that is showing
     * 4. get the showing lesson and show it
     * 5. if no lesson was showing
     * 6. get the next new lesson
     * 7. show it
     * 8. Mark it as showing
     * 9. if no new lessons
     * 10. loop through the lessons
     * 11. start over a min
     */
    if (true == lessons.connected())
    {
        if( (millis() - previous_millis) > FETCH_DATA_PERIOD )
        {
            static int currently_showing_row = 0;
            char lesson_buffer[128];
            char name_buffer[128];

            memset(lesson_buffer, 0, sizeof(lesson_buffer));
            memset(name_buffer, 0, sizeof(name_buffer));

            int showing_row = lessons.get_showing_row();

            Serial.print("Showing Row: ");
            Serial.println(showing_row);


            if ( showing_row > 1 )
            {

                if( (showing_row != currently_showing_row) &&
                    (true == show_lesson_and_name(lesson_buffer, sizeof(lesson_buffer), name_buffer, sizeof(name_buffer), showing_row)) )
                {
                    currently_showing_row = showing_row;
                }
            }
            else
            {
                int next_row = lessons.get_next_new_row();
                Serial.print("Next New Row: ");
                Serial.println(showing_row);

                if ( next_row > 1 )
                {
                    if ( (next_row != currently_showing_row) &&
                         (true == show_lesson_and_name(lesson_buffer, sizeof(lesson_buffer), name_buffer, sizeof(name_buffer), next_row)) )
                    {
                        lessons.set_as_showing(next_row);
                        currently_showing_row = next_row;
                    }
                }
                else
                {
                    static int row_counter = 2;
                    int number_of_rows = lessons.get_total_number_row();

                    if( true == show_lesson_and_name(lesson_buffer, sizeof(lesson_buffer), name_buffer, sizeof(name_buffer), row_counter) )
                    {
                        currently_showing_row = row_counter;
                    }

                    row_counter++;

                    if( row_counter > number_of_rows )
                    {
                        row_counter = 2;
                    }
                }
            }
            Serial.print("In 2022, I learnt: ");
            Serial.println(lesson_buffer);
            Serial.print("From: ");
            Serial.println(name_buffer);
            Serial.println("........................................................................................");
            previous_millis = millis();
            
        }
    }
    else
    {
        lessons.reconnect();
        Serial.println("....");
        delay(1000);
    }
}

bool show_lesson_and_name(char *lesson, uint32_t lesson_len, char *name, uint32_t name_len, int row)
{
    bool stat = false;

    if ((true == lessons.get_lesson(lesson, lesson_len, row)) &&
        (true == lessons.get_name(name, name_len, row)))
    {
        // show the display
        screen.update(lesson, strlen(lesson), name, strlen(name));
        pix.generate_colors_from_message(lesson, strlen(lesson));
        pix.run();
        stat = true;
    }

    return stat;
}
