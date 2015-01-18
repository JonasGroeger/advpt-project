#include "DepthFirstBranchAndBound.hpp"
#include "BuildListTester.hpp"

DepthFirstBranchAndBound::DepthFirstBranchAndBound()
{
}

void DepthFirstBranchAndBound::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("DFBB Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);
    //tester.writeBuildList(list, "buildlist.txt");
    //BuildLogResult result = tester.analyzeBuildLog(log, "scv", "rush");

   // LOG_DEBUG("DFFB Algorithm finished time: " << result.executionTime << " result: " << result.result);
}
