#include <entities/Worker.hpp>

void Worker::setTypeOfWork(TypeOfWork work)
{
    currentWork = work;
}

TypeOfWork Worker::getTypeOfWork()
{
    return currentWork;
}
