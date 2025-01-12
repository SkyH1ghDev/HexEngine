#include "ExitHandler.hpp"

ExitHandler::ExitHandler() : IObserver()
{

}

void ExitHandler::Update(std::any data)
{
    ExitData exitData = std::any_cast<ExitData>(data);

    this->_shouldTerminate = exitData.ShouldTerminate;
}