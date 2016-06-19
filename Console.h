#pragma once


//HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
//HANDLE std_input = GetStdHandle( STD_INPUT_HANDLE );


class Console
{
public:

    static void show_cursor( BOOL visible = true )
    {
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo( std_output, &cursor_info );

        if ( visible != cursor_info.bVisible )
        {
            cursor_info.bVisible = visible;
            SetConsoleCursorInfo( std_output, &cursor_info );
        }
    }

    static void fix_utf8_output()
    {
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbinfo;
        GetConsoleScreenBufferInfo( std_output, &csbinfo );
        COORD coord = { 0, 0 };
        CHAR_INFO chiFill;
        chiFill.Char.UnicodeChar = L' ';
        ScrollConsoleScreenBuffer( std_output, &csbinfo.srWindow, NULL, coord, &chiFill  );
    }

    static void print_font_info()
    {
        CONSOLE_FONT_INFOEX f;
        f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );

        if ( 0 == GetCurrentConsoleFontEx( std_output, TRUE, &f ) )
        {
            std::cout << "failed: " << GetLastError() << std::endl;
            return;
        }

        std::wcout
            << L"nFont     : " << f.nFont << std::endl
            << L"dwFontSize: " << f.dwFontSize.X << L"," << f.dwFontSize.Y << std::endl
            << L"FontFamily: " << f.FontFamily << std::endl
            << L"FontWeight: " << f.FontWeight << std::endl
            << L"FaceName  : " << f.FaceName << std::endl;
    }

    static void set_font( SHORT font_size = 14, const std::wstring& face_name = L"Consolas" )
    {
        // Console, Lucida Console, 新宋体
        SetConsoleOutputCP( face_name == L"新宋体" ? 936 : CP_UTF8 );
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_FONT_INFOEX f;
        f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
        f.nFont = 0;
        f.dwFontSize.X = 0;
        f.dwFontSize.Y = font_size;
        f.FontFamily = FF_DONTCARE;
        f.FontWeight = FW_NORMAL;
        wcscpy_s( f.FaceName, face_name.c_str() );
        SetCurrentConsoleFontEx( std_output, FALSE, &f );
    }

    static void disable_close_button()
    {   
        HWND w = GetConsoleWindow();
        HMENU m = GetSystemMenu( w, FALSE );
        DeleteMenu( m, SC_CLOSE , MF_BYCOMMAND );
        DeleteMenu( m, SC_MINIMIZE , MF_BYCOMMAND );
        DeleteMenu( m, SC_MAXIMIZE , MF_BYCOMMAND );
        DrawMenuBar( w );
    }

    static void set_color( WORD color )
    {
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        DWORD written = 0;
        COORD coord = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( std_output, &csbi );
        csbi.wAttributes = color;
        SetConsoleTextAttribute( std_output, csbi.wAttributes );
        FillConsoleOutputAttribute( std_output, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &written ); 
    }

    static void set_window( SHORT col, SHORT row )
    {
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        COORD coord = { 0, 0 };
        COORD size = { col, row };
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( std_output, &csbi );

        // make sure buffer size is bigger window size
        COORD cur_size = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
        COORD tmp_buf_size = { std::max<int>(cur_size.X, col), std::max<int>(cur_size.Y, row) };
        SetConsoleScreenBufferSize( std_output, tmp_buf_size );

        SMALL_RECT new_window = csbi.srWindow;
        new_window.Right = new_window.Left + col - 1;
        new_window.Bottom = new_window.Top + row - 1;
        SetConsoleWindowInfo( std_output, TRUE, &new_window );

        SetConsoleScreenBufferSize( std_output, size );
    }
};
