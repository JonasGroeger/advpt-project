#include "DFBB.hpp"
#include "BuildListTester.hpp"
#include <vector>

DFBB::DFBB() { }
void DFBB::run(char* entity, char* mode, int maxSimulationTime) {
    cout << "Algorithm started";
    BuildListTester tester;
    vector<BuildStep*> list;
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
    BuildLogResult result = tester.analyzeBuildLog(log, "scv", "rush" );
    cout << "time: " << result.executionTime << " result: " << result.result;;

}