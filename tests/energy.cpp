#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "State.h"
#include "ConfigParser.h"
#include "BuildOrder.h"

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

void testEnergyManager()
{
    EnergyManager em;
    ConfigParser::Instance().setRaceForAction("command_center");
    const BuildAction& act = ConfigParser::Instance().getAction("orbital_command");
    action_t a = act.id;

    em.registerNew(a, 50, 100);
    em.registerNew(a, 50, 100);
    em.registerNew(a, 50, 100);
    em.registerNew(a, 50, 100);

    cerr << em << endl;
    time_t t = em.timeUntilEnergyIsAvailable(a, 75);
    cerr << "time until 75 Energy is available: " << t << endl;

    em.advanceTime(t);

    cerr << em << endl;

    em.consumeEnergy(a, 75);
    em.consumeEnergy(a, 75);
    em.consumeEnergy(a, 75);
    
    cerr << em << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);
    ConfigParser::Instance().setRaceForAction("command_center");
    BuildOrder b {"scv", "refinery", "supply_depot", "barracks" , "orbital_command", "mule", "mule", "mule", "mule"};
    BuildOrder b2 {"scv", "refinery", "supply_depot", "barracks" , "orbital_command", 
        "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "marine", "supply_drop", "marine"};


    State muleTest(ConfigParser::Instance().getStartConfig());
    cerr << "initial state: " << muleTest;

    vector<string> actions = {"supply_depot", "barracks", "orbital_command"};
    for (string a : actions)
    {
        const BuildAction& act = ConfigParser::Instance().getAction(a);
        time_t t;
        while ( (t = muleTest.isAdditionalTimeNeeded(act)) )
        {
            muleTest.advanceTime(t);
        }
        muleTest.startAction(act);
    }
    muleTest.advanceTime(muleTest.getTimeTillAllActionsAreFinished());
    cerr << muleTest << endl;

    State noMule(muleTest);
    noMule.advanceTime(90);

    muleTest.startAction(ConfigParser::Instance().getAction("mule"));

    cerr << muleTest << endl;
    muleTest.advanceTime(45);
    cerr << muleTest << endl;
    muleTest.advanceTime(45);
    cerr << muleTest << endl;
    cerr << noMule << endl;

    ress_t diff = muleTest.minerals - noMule.minerals;
    ress_t diff2 = 4 * 90 * 70;
    assert(diff == diff2);

    ConfigParser::Instance().setRaceForAction("chrono_boost");
    State chronoBoost(ConfigParser::Instance().getStartConfig());

    actions = {"pylon", "gateway"};
    for (string a : actions)
    {
        const BuildAction& act = ConfigParser::Instance().getAction(a);
        time_t t;
        while ( (t = chronoBoost.isAdditionalTimeNeeded(act)) )
        {
            chronoBoost.advanceTime(t);
        }
        chronoBoost.startAction(act);
    }

    State noChrono(chronoBoost);
    chronoBoost.startAction(ConfigParser::Instance().getAction("chrono_boost"));

    const BuildAction& zealot = ConfigParser::Instance().getAction("zealot");

    chronoBoost.advanceTime(70);
    noChrono.advanceTime(70);

    chronoBoost.startAction(zealot);
    noChrono.startAction(zealot);

    chronoBoost.advanceTime(chronoBoost.getTimeTillAllActionsAreFinished());
    noChrono.advanceTime(noChrono.getTimeTillAllActionsAreFinished());

    assert(noChrono.currentTime - chronoBoost.currentTime == 10);

    chronoBoost.startAction(ConfigParser::Instance().getAction("assimilator"));
    assert(chronoBoost.isLegalAction(ConfigParser::Instance().getAction("chrono_boost")));

    cerr << "SUCCESS" << endl;
    return 0;
}
