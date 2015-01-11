#include "DepthFirstBranchAndBound.hpp"
#include "BuildListTester.hpp"

DepthFirstBranchAndBound::DepthFirstBranchAndBound()
{
}

void DepthFirstBranchAndBound::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("DFBB Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);

    BuildListTester tester;
    vector<BuildStep *> list;

    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("supply_depot"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("barracks"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("scv"));
    list.push_back(new BuildStep("marine"));

    vector<string> log = tester.testBuildList(list);
    tester.writeLog(log, "logfile.txt");
    //tester.writeBuildList(list, "buildlist.txt");
    //BuildLogResult result = tester.analyzeBuildLog(log, "scv", "rush");

   // LOG_DEBUG("DFFB Algorithm finished time: " << result.executionTime << " result: " << result.result);
}
