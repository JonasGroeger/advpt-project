#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "State.h"
#include "ConfigParser.h"

using namespace std; // If you feel offended by this change it!

int i = 0;

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
    testZerg();
    testProtoss();
    /*
    cerr << "Adding command_center" << endl;
    s.addActionResult(ConfigParser::Instance().getAction("command_center").result, false);
    cerr << "Adding scv" << endl;
    s.addActionResult(ConfigParser::Instance().getAction("scv").result, false);
    s.supply_used ++;

    cerr << "Created state with a command_center and one scv" << endl;

    simple_test("It is possible to build a scv", s.isLegalAction(ConfigParser::Instance().getAction("scv")), true);
    simple_test("It is possible to build a supply_depot", s.isLegalAction(ConfigParser::Instance().getAction("supply_depot")), true);
    simple_test("It is NOT possible to build a barracks", s.isLegalAction(ConfigParser::Instance().getAction("barracks")), false);
    simple_test("Currently producing 0.7 minerals/second", s.getMineralsPerTick(), (ress_t)70);
    simple_test("Currently producing 0 gas/second", s.getGasPerTick(), (ress_t)0);
    simple_test("supply_depot can be built in 143 ticks", s.isAdditionalTimeNeeded(ConfigParser::Instance().getAction("supply_depot")), (time_t)143);

    cerr << s << endl;

    cerr << "Advancing time by 143" << endl;
    s.advanceTime(143);
    
    cerr << s << endl;

    cerr << "Starting supply_depot" << endl;
    s.startAction(ConfigParser::Instance().getAction("supply_depot"));

    cerr << s << endl;

    cerr << "Advancing time by 32" << endl;
    s.advanceTime(32);

    cerr << s << endl;

    cerr << "Adding 10 scv's" << endl;
    for (int i = 0; i < 10; i++)
    {
        s.addActionResult(ConfigParser::Instance().getAction("scv").result, false);
        s.supply_used ++;
    }

    cerr << s << endl;

    cerr << "Advancing time by 200" << endl;
    s.advanceTime(200);
    
    cerr << s << endl;

    cerr << "Starting refinery" << endl;
    s.startAction(ConfigParser::Instance().getAction("refinery"));
    
    cerr << s << endl;

    cerr << "Advancing time by 3" << endl;
    s.advanceTime(3);
    
    cerr << s << endl;

    cerr << "Starting supply_depot" << endl;
    s.startAction(ConfigParser::Instance().getAction("supply_depot"));
    
    cerr << s << endl;

    cerr << "Advancing time by 5" << endl;
    s.advanceTime(5);
    
    cerr << s << endl;

    cerr << "Starting barracks" << endl;
    s.startAction(ConfigParser::Instance().getAction("barracks"));

    cerr << s << endl;
    
    cerr << "Advancing time by 65" << endl;

    s.advanceTime(65);
    
    cerr << s << endl;

    cerr << "Let's produce 10 marines" << endl;

    simple_test("Marine's are generally possible", s.isLegalAction(ConfigParser::Instance().getAction("marine")), true);

    for (int i = 0; i < 10; i++)
    {
        // this is not the correct way to do this...
        cerr << s << endl;
        time_t t = s.isAdditionalTimeNeeded(ConfigParser::Instance().getAction("marine"));
        cerr << "Time till next marine is possible: " << t << endl;
        cerr << "Advancing time by " << t << endl;
        s.advanceTime(t);
        s.startAction(ConfigParser::Instance().getAction("marine"));
    }

    cerr << s << endl;



    */

    cerr << "SUCCESS" << endl;
    return 0;
}
