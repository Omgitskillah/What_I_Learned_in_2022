#include <Arduino.h>
#include <HTTPClient.h>
#include "lesson_manager.h"
#include "credentials.h"

String lesson_manager_url = "https://script.google.com/macros/s/" + GoogleAppID + "/exec";
String get_row_count = "?command=get_row_count";
String get_showing_row_params = "?command=get_showing";
String get_next_new_row_params = "?command=get_nex_new";
String get_lesson_params = "?command=get_lesson&row=";
String get_name_params = "?command=get_name&row=";
String set_as_showing_params = "?command=set_to_showing&row=";

const char *ssid = mySSID;         // change SSID
const char *password = myPassword; // change password

bool lesson_manager::get_lesson( char * buffer, uint32_t buffer_len, int row )
{
    String url = lesson_manager_url + get_lesson_params + String(row);
    return get_lesson_data( url, buffer, buffer_len );
}

bool lesson_manager::get_name( char * buffer, uint32_t buffer_len, int row )
{
    String url = lesson_manager_url + get_name_params + String(row);
    return get_lesson_data( url, buffer, buffer_len );
}

bool lesson_manager::set_as_showing( int row )
{
    String url = lesson_manager_url + set_as_showing_params + String(row);
    return get_lesson_data( url, NULL, 0 );
}

int lesson_manager::get_showing_row( void )
{
    int showing_row = 0;
    String url = lesson_manager_url + get_showing_row_params;
    if( true == get_lesson_data(url, response_buffer, sizeof(response_buffer)) )
    {
        String result = String(response_buffer);
        showing_row = result.toInt();
    }

    return showing_row;
}

int lesson_manager::get_next_new_row( void )
{
    int next_new_row = 0;
    String url = lesson_manager_url + get_next_new_row_params;
    if( true == get_lesson_data(url, response_buffer, sizeof(response_buffer)) )
    {
        String result = String(response_buffer);
        next_new_row = result.toInt();
    }

    return next_new_row;
}

int lesson_manager::get_total_number_row( void )
{
    int number_of_rows = 0;
    String url = lesson_manager_url + get_row_count;
    if( true == get_lesson_data(url, response_buffer, sizeof(response_buffer)) )
    {
        String result = String(response_buffer);
        number_of_rows = result.toInt();
    }

    return number_of_rows;
}

bool lesson_manager::get_lesson_data( String url, char * buffer, uint32_t buffer_len )
{
    bool success = false;

    HTTPClient http;

    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    int httpCode = http.GET();

    if (200 == httpCode)
    {
        String payload = http.getString();

        if ((NULL != buffer) &&
            (payload.length() < buffer_len))
        {
            memset(buffer, 0, buffer_len);
            memcpy(buffer, payload.c_str(), payload.length());
        }
        success = true;
    }
    else
    {
        Serial.print("HTTP Error Status: ");
        Serial.println(httpCode);
    }

    http.end();
    return success;
}

void lesson_manager::reconnect( void )
{
    WiFi.begin(ssid, password);
    Serial.println();
    Serial.print("Connecting to wifi: ");
    Serial.println(ssid);
    Serial.flush();
}

bool lesson_manager::connected( void )
{
    return (WiFi.status() == WL_CONNECTED);
}
