#pragma once

#include <vector>

#include "Debug.hpp"

class DFBB
{
public:
    DFBB();

    void run(char *entity, char *mode, int maxSimulationTime);
};
