#include "BuildOrder.hpp"

BuildOrder::BuildOrder(const char *file)
{
    ifstream infile(file);
    string line;
    while (getline(infile, line))
    {
        buildSteps.push_back(new BuildStep(line));
    }

    iterator = buildSteps.begin();
}

BuildOrder::~BuildOrder()
{
    std::for_each(buildSteps.begin(), buildSteps.end(), [](BuildStep* bs) {delete bs;});
}

BuildStep* BuildOrder::getNextStep()
{
    if(iterator != buildSteps.end())
    {
        return *iterator;
    }
    return nullptr;
}

bool BuildOrder::isDone()
{
        return iterator == buildSteps.end();
}

void BuildOrder::advance()
{
    if (iterator != buildSteps.end())
    {
        ++iterator;
    }
}
