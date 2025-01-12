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

//----------------------------------------------------------------------------//

#define MSG_LOG_MAC(m) spdlog::info("{} - {} : Line {}",m ,__FILENAME__, __LINE__)

#define WARN_LOG_MAC(m) spdlog::warn("{} - {} : Line {}",m ,__FILENAME__ ,__LINE__)

#define ERROR_LOG_MAC(m)                                          \
    spdlog::error("{} - {} : Line {}",m ,__FILENAME__ ,__LINE__); \
    std::exit(EXIT_FAILURE);                                      \

#define GLFW_ERR_LOG_MAC(err, msg)                       \
    spdlog::error("GLFW Err Code [{}] - {}",err ,msg);   \
    std::exit(EXIT_FAILURE);                             \

#define FILE_IO_ERR_LOG_MAC(file)            \
    spdlog::error("FILE IO ERR - {}",file);  \
    std::exit(EXIT_FAILURE);                 \

#endif