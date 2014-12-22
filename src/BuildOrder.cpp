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

    doSanityCheck();
}

BuildOrder::~BuildOrder()
{
    std::for_each(buildSteps.begin(), buildSteps.end(), [](BuildStep* bs) {delete bs;});
}

bool BuildOrder::doSanityCheck()
{
    if (buildSteps.size() == 0)
    {
        throw std::invalid_argument("There are no steps in the build list!");
    }

    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());
    for (BuildStep* step : buildSteps)
    {
        EntityType stepType = step->getEntityType();
        if (stepType == NONE)
        {
            throw std::invalid_argument(step->getName());
            throw std::invalid_argument("Did not recognize entityType");
        }

        if (this->race != Entity::typeToRace(stepType))
        {
            throw std::invalid_argument("Race mismatch: TODO MEANINGFUL ERROR MESSAGE");
        }
    }

    // TODO calculate max time and set this in BuildOrder maybe?
    // TODO calculate if supply is possible
    // TODO calculate if basic dependencies are possible

    return true;
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

EntityType BuildOrder::getRace()
{
    return this->race;
}
