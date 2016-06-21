#pragma once


namespace Utility
{
    HANDLE cout();
    HANDLE cin();
    void show_console_cursor( BOOL visible = true, HANDLE output_handle = cout() );
    void refresh_console_window( HANDLE output_handle = cout() );
    void set_console_font( SHORT font_size = 14, const std::wstring& face_name = L"������", HANDLE output_handle = cout() );
    void disable_console_system_buttons();
    void set_console_color( WORD color, HANDLE output_handle = cout() );
    void set_console_window_size( SHORT col, SHORT row, HANDLE output_handle = cout() );
    int WINAPI console_ctrl_handler( DWORD  dwCtrlType );
    void set_console_ctrl_handler();
    void disable_console_mode( DWORD disable_mode, HANDLE handle = cout() );
    void cls( HANDLE output_handle = cout() );
    void cls2( HANDLE output_handle = cout() );
    void write_console( const std::string& s, int code_page = CP_UTF8, HANDLE output_handle = cout() );
    void write_console( const std::wstring& ws, HANDLE output_handle = cout() );
    void write_console_on_center( const std::string& s, HANDLE output = GetStdHandle( STD_OUTPUT_HANDLE ) );
    std::wostream& print_font_info( std::wostream&, const CONSOLE_FONT_INFOEX& info );
}
