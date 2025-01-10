#ifndef LOGGING_H
#define LOGGING_H

#include <spdlog/spdlog.h>
#include <string.h>

//----------------------------------------------------------------------------//

#ifdef _WIN32
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

// #define EXIT

#endif