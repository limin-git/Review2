#pragma once


namespace Utility
{
    std::vector<std::string> split_string( const std::string& s, const std::string& separator = ",:;/%~-\t|" );
    std::vector<std::string> extract_strings_in_braces( const std::string& s );
}
