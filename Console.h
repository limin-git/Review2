#pragma once


class Console
{
public:

    static void show_cursor ( BOOL visible = true )
    {
        HANDLE stdout_handle = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_CURSOR_INFO cursor_info ;
        GetConsoleCursorInfo( stdout_handle, &cursor_info );

        if ( visible != cursor_info.bVisible )
        {
            cursor_info.bVisible = visible;
            SetConsoleCursorInfo( stdout_handle, &cursor_info );
        }
    }

};
