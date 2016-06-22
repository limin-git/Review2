#include "stdafx.h"
#include "Utility.h"


namespace Utility
{

    size_t random_number( size_t lo, size_t hi )
    {
        static boost::random::mt19937 gen( static_cast<boost::uint32_t>( std::time(0) ) );
        static boost::random::uniform_int_distribution<> dist;
        size_t x = dist( gen );

        if ( lo != std::numeric_limits <size_t> ::min() || hi != std::numeric_limits <size_t> ::max() )
        {
            while ( x < lo || hi < x )
            {
                x %= hi;
                x += lo;
            }
        }

        //LOG_TRACE << x << "(low = " << lo << ", high = " << hi << ")";
        return x;
    }

}
