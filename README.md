# Starcraft 2 build order simulator

# Mailing list
advpt@googlegroups.com

# Formatting
    #define MAX_LINE_LENGTH  120
    #define INDENT_USING     SPACES
    #define INDENT_COUNT     4
    #define BRACKETS_IN      NEWLINE

# Log
    <simulation time in seconds>  <message type>   <additional info (depends on message type) >
    Example:
    0    build-start   probe
    0    workers       minerals:6,vespene:0
    17   build-end     probe
    17   build-start   probe
    17   workers       minerals:7,vespene:0
    34   build-end     probe
    ...
    60   build-start   pylon
    60   workers       minerals:9,vespene:0
    ...
    320  special       chronoboost:gateway
    320  workers       minerals:9,vespene:3
    ...
    350 resources      minerals:80,vespene:20,usedSupply:30,availableSupply:40

	Message Types:
	    built-start  <building or unit >
	    build-end    <building or unit >
	    workers      minerals:<nrOfWorkersHarvestingMinerals>,vespene:<nrOfWorkersHarvestingVespene>
	    special      chronoboost:<buildingThatChronoBoostActsOn>
	    special      extrasupplies
	    special      mule
	    special      injectlarvae
	    resources    minerals:<minerals>,vespene:<vespene>,usedSupply:<usedSupply>,availableSupply:<availableSupply>

For each reported time, the last message has to be a "worker" message. i.e. when something happens at t=30 (build start, build finished) then there has to be also a "worker" message as last message for t=30

For this assignment you do not have to model and use the special abilities for races, but if you do, you have to print the according log message the resources message has to be printed at the end only (but for debugging may also be printed during the simulation) you can use additional custom message types for debugging. If you do so, please document them in a README.txt
