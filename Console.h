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
        // nFont: (no use)
        // 14 Console
        // 15 Lucida Console
        // 6  ÐÂËÎÌå
        // dwFontSize   Consolas    Lucida Console  ÐÂËÎÌå
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
        // ÐÂËÎÌå

        SetConsoleOutputCP( face_name == L"ÐÂËÎÌå" ? 936 : CP_UTF8 ); // DO NOT use CP_ACP
        HANDLE std_output = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_FONT_INFOEX f;
        f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
        f.nFont = 0;
        f.dwFontSize.X = 0;
        f.dwFontSize.Y = font_size;
        f.FontFamily = FF_DONTCARE;
        f.FontWeight = FW_NORMAL;
        wcscpy_s( f.FaceName, face_name.c_str() );

        if ( 0 == SetCurrentConsoleFontEx( std_output, FALSE, &f ) )
        {
            std::cout << "failed: " << GetLastError() << std::endl;
            return;
        }
    }

};
