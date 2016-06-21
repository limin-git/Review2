#include "stdafx.h"
#include "ConsoleUtility.h"
#include "UnicodeUtility.h"


namespace Utility
{

    HANDLE cout()
    {
        HANDLE output = GetStdHandle( STD_OUTPUT_HANDLE );
        return output;
    }


    HANDLE cin()
    {
        HANDLE input = GetStdHandle( STD_INPUT_HANDLE );
        return input;
    }


    void show_console_cursor( BOOL visible, HANDLE output_handle )
    {
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo( output_handle, &cursor_info );

        if ( visible != cursor_info.bVisible )
        {
            cursor_info.bVisible = visible;
            SetConsoleCursorInfo( output_handle, &cursor_info );
        }
    }


    void refresh_console_window( HANDLE output_handle )
    {
        CONSOLE_SCREEN_BUFFER_INFO csbinfo;
        GetConsoleScreenBufferInfo( output_handle, &csbinfo );
        COORD coord = { 0, 0 };
        CHAR_INFO char_fill;
        char_fill.Char.UnicodeChar = L' ';
        ScrollConsoleScreenBuffer( output_handle, &csbinfo.srWindow, NULL, coord, &char_fill  );
    }


    void set_console_font( SHORT font_size, const std::wstring& face_name, HANDLE output_handle )
    {
        // nFont: (no use)
        // 14 Console
        // 15 Lucida Console
        // 6  新宋体
        // dwFontSize   Consolas    Lucida Console  新宋体
        // 10:          5,  10      6,  10          5,  11
        // 12:          6,  12      7,  12          6,  14
        // 14:          7,  14      8,  14          7,  16
        // 16:          8,  16      10, 16          8,  18
        // 18:          8,  18      11, 18          9,  21
        // 20:          9,  20      12, 20          10, 23
        // 24:          11, 24      14, 24          12, 27
        // 28:          13, 28      17, 28          14, 32
        // 36:          17, 36      22, 36          18, 41
        // FaceName:
        // Lucida Console
        // Consolas
        // Terminal
        // 新宋体

        SetConsoleOutputCP( face_name == L"新宋体" ? 936 : CP_UTF8 ); // DO NOT use CP_ACP
        CONSOLE_FONT_INFOEX f;
        f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
        f.nFont = 0;
        f.dwFontSize.X = 0;
        f.dwFontSize.Y = font_size;
        f.FontFamily = FF_DONTCARE;
        f.FontWeight = FW_NORMAL;
        wcscpy_s( f.FaceName, face_name.c_str() );

        if ( 0 == SetCurrentConsoleFontEx( output_handle, FALSE, &f ) )
        {
            std::cout << "failed: " << GetLastError() << std::endl;
            return;
        }
    }


    void disable_console_system_buttons()
    {   
        HWND w = GetConsoleWindow();
        HMENU m = GetSystemMenu( w, FALSE );
        DeleteMenu( m, SC_CLOSE , MF_BYCOMMAND );
        DeleteMenu( m, SC_MINIMIZE , MF_BYCOMMAND );
        DeleteMenu( m, SC_MAXIMIZE , MF_BYCOMMAND );
        DrawMenuBar( w );
    }


    void set_console_color( WORD color, HANDLE output_handle )
    {
        DWORD written = 0;
        COORD coord = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( output_handle, &csbi );
        csbi.wAttributes = color;
        SetConsoleTextAttribute( output_handle, csbi.wAttributes );
        FillConsoleOutputAttribute( output_handle, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &written ); 
    }


    void set_console_window_size( SHORT col, SHORT row, HANDLE output_handle )
    {
        COORD coord = { 0, 0 };
        COORD size = { col, row };
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( output_handle, &csbi );

        // make sure buffer size is bigger window size
        COORD cur_size = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
        COORD tmp_buf_size = { std::max<int>(cur_size.X, col), std::max<int>(cur_size.Y, row) };
        SetConsoleScreenBufferSize( output_handle, tmp_buf_size );

        SMALL_RECT new_window = csbi.srWindow;
        new_window.Right = new_window.Left + col - 1;
        new_window.Bottom = new_window.Top + row - 1;
        SetConsoleWindowInfo( output_handle, TRUE, &new_window );

        SetConsoleScreenBufferSize( output_handle, size );
    }


    int WINAPI console_ctrl_handler( DWORD  dwCtrlType )
    {
        std::cout << static_cast<char>(7) << std::endl;

        switch ( dwCtrlType )
        {
        case CTRL_C_EVENT:
            std::cout << "Ctrl + C" << std::endl;
            break;
        case CTRL_BREAK_EVENT:
            std::cout << "Ctrl + BREAK" << std::endl;
            break;
        case CTRL_CLOSE_EVENT:
            std::cout << "CLOSE EVENT" << std::endl;
            break;
        case CTRL_LOGOFF_EVENT:
            std::cout << "LOGOFF EVENT" << std::endl;
            break;
        case CTRL_SHUTDOWN_EVENT:
            std::cout << "SHUTDOWN EVENT" << std::endl;
            break;
        }

        return TRUE;
    }


    void set_console_ctrl_handler()
    {
        SetConsoleCtrlHandler( console_ctrl_handler, TRUE );
    }


    void disable_console_mode( DWORD disable_mode, HANDLE handle )
    {
        // MUST call this before SetConsoleMode
        DWORD mode = 0;
        GetConsoleMode( handle, &mode );
        mode &= ~disable_mode;
        SetConsoleMode( handle, mode );
    }


    void cls( HANDLE output_handle )
    {
        COORD coord = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFOEX csbi;
        csbi.cbSize = sizeof( CONSOLE_SCREEN_BUFFER_INFOEX );
        GetConsoleScreenBufferInfoEx( output_handle, &csbi );
        size_t buf_size = csbi.dwSize.X * csbi.dwSize.Y;
        CHAR_INFO* buf = new CHAR_INFO[buf_size];
        for ( size_t i = 0; i < buf_size; ++i )
        {
            buf[i].Attributes = csbi.wAttributes;
            buf[i].Char.UnicodeChar = L' ';
        }
        SMALL_RECT wrige_region = { 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1 };
        WriteConsoleOutput( output_handle, buf, csbi.dwSize, coord, &wrige_region );
        delete[] buf;
        buf = NULL;
    }


    void cls2( HANDLE output_handle )
    {
        DWORD written = 0;
        COORD coord = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( output_handle, &csbi );
        size_t buf_size = csbi.dwSize.X * csbi.dwSize.Y;
        wchar_t* buf = new wchar_t[buf_size];
        for ( size_t i = 0; i < buf_size; ++i )
        {
            buf[i] = L' ';
        }
        WriteConsoleOutputCharacterW( output_handle, buf, buf_size, coord, &written );
    }


    void write_console( const std::string& s, int code_page, HANDLE output_handle )
    {
        std::wstring ws = to_wstring( s, code_page );
        WriteConsoleW( output_handle, ws.c_str(), ws.size(), 0, 0 );
    }


    void write_console( const std::wstring& ws, HANDLE output_handle )
    {
        WriteConsoleW( output_handle, ws.c_str(), ws.size(), 0, 0 );
    }


    void write_console_on_center( const std::string& s, HANDLE output )
    {
        std::wstring ws = to_wstring( s, CP_UTF8 );
        std::string as = to_string( ws, 936 );

        DWORD written = 0;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( output, &csbi );
        size_t window_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size_t window_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if ( 0 == ( window_height % 2 ) )
        {
            --window_height;
        }

        size_t pos = ( window_width * window_height - as.size() ) / 2;
        COORD coord = { pos % window_width, pos / window_width };
        WriteConsoleOutputCharacterW( output, ws.c_str(), ws.size(), coord, &written );
    }


    std::wostream& print_font_info( std::wostream& os, const CONSOLE_FONT_INFOEX& i )
    {
        return os
            << L"nFont     : " << i.nFont << std::endl
            << L"dwFontSize: " << i.dwFontSize.X << L"," << i.dwFontSize.Y << std::endl
            << L"FontFamily: " << i.FontFamily << std::endl
            << L"FontWeight: " << i.FontWeight << std::endl
            << L"FaceName  : " << i.FaceName << std::endl;
    }

}
