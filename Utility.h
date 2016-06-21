#pragma once
#include "TimeUtility.h"
#include "StringUtility.h"
#include "UnicodeUtility.h"
#include "SoundUtility.h"
#include "ConsoleUtility.h"

#ifdef max
#undef max
#undef min
#endif


namespace Utility
{
    size_t random_number( size_t lo = std::numeric_limits <size_t>::min(), size_t hi = std::numeric_limits <size_t>::max() );
}
