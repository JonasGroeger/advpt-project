#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG_DEBUG(msg) (std::cerr << "[D] - [" << __PRETTY_FUNCTION__ << "]   " << msg << std::endl)
#else
#define LOG_DEBUG(msg)
#endif
