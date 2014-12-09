#pragma once

class BuildOrder
{
public:
        BuildStep getNextStep();
        void advance();        
}
