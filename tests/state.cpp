#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "State.h"
#include "ConfigParser.h"

using namespace std; // If you feel offended by this change it!

int i = 0;
ConfigParser &cfg = ConfigParser::Instance();

template <class T>
void simple_test(string msg, T expression, T expected_value)
{
    cerr << "TEST NR." << i++;
    cerr << " " << msg << endl;

    assert(expression == expected_value);
}

void assertOnlyLegalActions(State &s, vector<BuildAction> legals)
{
        for (BuildAction act : ConfigParser::Instance().getAllActions())
        {
                if (std::find(legals.begin(), legals.end(), act) == legals.end())
                {
                        simple_test(act.name + " should not be possible", s.isLegalAction(act), false);
                }
                else
                {
                        simple_test(act.name + " should be possible", s.isLegalAction(act), true);
                }
        }
}

vector<BuildAction> stringsToBuildActions(vector<string> strings)
{
        vector<BuildAction> buildActions;
        for (string s : strings)
        {
                buildActions.push_back(ConfigParser::Instance().getAction(s));
        }
        return buildActions;
}

void testTerran()
{
    ConfigParser::Instance().setRaceForAction("command_center");

    State terranState = State(ConfigParser::Instance().getStartConfig());
    cerr << terranState << endl;

    assertOnlyLegalActions(terranState, stringsToBuildActions({"command_center", "scv", "refinery", "supply_depot", "engineering_bay"}));

    terranState.advanceTime(terranState.isAdditionalTimeNeeded(cfg.getAction("supply_depot")));
    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.advanceTime(terranState.getTimeTillNextActionIsFinished());
    simple_test("count of supply_depot == 1", terranState.getEntityCount(cfg.getAction("supply_depot").id), 1);
    terranState.advanceTime(1000);

    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.startAction(cfg.getAction("supply_depot"));
    terranState.startAction(cfg.getAction("supply_depot"));

    simple_test("No worker available", terranState.isLegalAction(cfg.getAction("supply_depot")), false);
    terranState.startAction(cfg.getAction("scv"));
    simple_test("Worker will be available", terranState.isLegalAction(cfg.getAction("supply_depot")), true);
    ress_t future = terranState.future_supply_max;

    terranState.advanceTime(terranState.getTimeTillAllActionsAreFinished());

    simple_test("future_supply_max is supply_max after getTimeTillAllActionsAreFinished", future, terranState.supply_max);
    
    terranState.startAction(cfg.getAction("barracks"));
    terranState.advanceTime(terranState.getTimeTillAllActionsAreFinished());

    assertOnlyLegalActions(terranState, stringsToBuildActions({"command_center", "scv", "refinery", "supply_depot", "engineering_bay", "barracks", "marine", "bunker", "orbital_command"}));
    terranState.startAction(cfg.getAction("refinery"));
    terranState.advanceTime(terranState.getTimeTillAllActionsAreFinished());
    assertOnlyLegalActions(terranState, stringsToBuildActions({"command_center", "scv", "refinery", "supply_depot", "engineering_bay", "barracks", "marine", "bunker", "orbital_command", "barracks_reactor", "barracks_techlab", "factory", "ghost_academy"}));

    cerr << terranState << endl;

    cerr << "Resetting state" << endl;
    terranState = State(ConfigParser::Instance().getStartConfig());

    terranState.advanceTime(terranState.isAdditionalTimeNeeded(cfg.getAction("supply_depot")));
    terranState.startAction(cfg.getAction("supply_depot"));

    terranState.advanceTime(terranState.isAdditionalTimeNeeded(cfg.getAction("barracks")));
    terranState.advanceTime(terranState.isAdditionalTimeNeeded(cfg.getAction("barracks")));

    terranState.startAction(cfg.getAction("barracks"));

    terranState.advanceTime(terranState.getTimeTillAllActionsAreFinished());

    simple_test("count of supply_depot == 1", terranState.getEntityCount(cfg.getAction("supply_depot").id), 1);
    simple_test("count of barracks == 1", terranState.getEntityCount(cfg.getAction("barracks").id), 1);

    cerr << terranState << endl;
}
void testZerg()
{
    ConfigParser::Instance().setRaceForAction("hatchery");

    State zergState = State(ConfigParser::Instance().getStartConfig());
    cerr << zergState << endl;

    assertOnlyLegalActions(zergState, stringsToBuildActions({"overlord", "drone", "larva", "hatchery", "extractor", "spawning_pool", "evolution_chamber"}));
}
void testProtoss()
{
    ConfigParser::Instance().setRaceForAction("nexus");

    State protossState = State(ConfigParser::Instance().getStartConfig());
    cerr << protossState << endl;

    assertOnlyLegalActions(protossState, stringsToBuildActions({"nexus", "probe", "assimilator", "pylon"}));
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);

    testTerran();
    //testZerg();
    //testProtoss();

    cerr << "SUCCESS" << endl;
    return 0;
}
