#pragma once

#include "TypeOfWork.hpp"

class Worker
{
private:
    //every worker is idleing at the beginning
    TypeOfWork currentWork = TypeOfWork::Idle;

public:
    void setTypeOfWork(TypeOfWork work);

    TypeOfWork getTypeOfWork();
};
