#include <iostream>
#include "BuildOrder.hpp"

using namespace std;

int main(int argc, char** argv)
{
    BuildOrder* order = new BuildOrder(argv[1]);
    
    int count = 0;
    while(true){
        BuildStep* step = order->getNextStep();
        if(step == NULL){
            break;
        }
        cout << "[Step "<<count<<"] - " << step->getName() << endl;
        count++;
    }
    cout << "finished...\n";
    return 0;
}
