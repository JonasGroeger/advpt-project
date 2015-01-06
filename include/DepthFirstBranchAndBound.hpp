#pragma once

#include <vector>

#include "Debug.hpp"

class DepthFirstBranchAndBound
{
public:
    DepthFirstBranchAndBound();

    void run(char *entity, char *mode, int maxSimulationTime);
};
