#pragma once


class ConsoleCommand
{
public:

    ConsoleCommand();
    ~ConsoleCommand();
    void update_option( const boost::program_options::variables_map& vm );

public:

    void set_font_face_name( const std::string& name );
    void set_font_size( SHORT font_size );
    void set_console_width( SHORT width );
    void set_console_height( SHORT height );
    void set_console_size( SHORT col, SHORT row );
    void set_console_color( WORD color );
    void show_cursor( BOOL visible = true );
    void disable_close_button();

public:

    HANDLE m_std_output_handle;
    UINT m_cp;
    UINT m_output_cp;
};
