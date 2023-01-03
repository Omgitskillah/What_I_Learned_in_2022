#ifndef _LESSON_MANAGER_H_
#define _LESSON_MANAGER_H_

class lesson_manager
{ 
    private:
        bool connection_status;
        char response_buffer[32];
        bool get_lesson_data( String url, char * buffer, uint32_t buffer_len );
    public:
        void reconnect( void );
        bool connected( void );
        int get_total_number_row( void );
        int get_next_new_row( void );
        int get_showing_row( void );
        bool set_as_showing( int row );
        bool get_name( char * buffer, uint32_t buffer_len, int row );
        bool get_lesson( char * buffer, uint32_t buffer_len, int row );
};
#endif