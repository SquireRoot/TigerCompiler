/**
 * @file Common.h
 * @brief Contains common definitions for the tigerc project
 * @author Evan Newman, evanmnewman1@gmail.com
 * @date February 2025
 */

#ifndef TIGERC_COMMON_H_
#define TIGERC_COMMON_H_

// Includes
#include <iostream>

namespace tigerc {

extern bool enable_logging;

#define ErrorLogger(msg) \
    if (enable_logging) std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") " << msg << std::endl;

#define DebugLogger(msg) \
    if (enable_logging) std::cout << "(" << __FILE__ << ":" << __LINE__ << ") " << msg << std::endl;

enum Status {
    SUCCESS = 0,
    GENERAL_ERROR = 1,
    SCANNER_ERROR = 2,
    PARSER_ERROR = 3
};

}

#endif