#include "BuildOrder.hpp"

BuildStep* BuildOrder::getNextStep(){
    if(iterator != buildSteps.end()){
        ++iterator;
        return *iterator;
    }
    return nullptr;
}

void BuildOrder::advance(){

}
