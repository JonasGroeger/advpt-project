#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG_DEBUG(msg) //(std::cout << "[D] " << msg << std::endl)
#else
#define LOG_DEBUG(msg)
#endif
