#include "BuildOrder.hpp"

BuildStep* BuildOrder::getNextStep()
{
    if(iterator != buildSteps.end())
    {
        return *iterator;
    }
    return nullptr;
}

void BuildOrder::advance()
{
        if (iterator != buildSteps.end())
        {
            ++iterator;
        }
}
