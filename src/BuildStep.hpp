#pragma once

class BuildStep
{
private:
    BuildStepType type;
    Entity which;
public:
    BuildStepType getType();
    Entity getWhich();
};
