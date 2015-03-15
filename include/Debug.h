#pragma once

#include <iostream>

#ifdef DEBUG
// For __PRETTY_FUNCTION__, see http://stackoverflow.com/a/26907887/488265
#define LOG_DEBUG(msg) (std::cerr << "[D] - [" << __PRETTY_FUNCTION__ << "]   " << msg << std::endl)
#else
#define LOG_DEBUG(msg)
#endif
