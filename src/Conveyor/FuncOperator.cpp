#include <utility>

#include "Conveyor/FuncOperator.hpp"

Conveyor::FuncOperator::FuncOperator(Conveyor::FuncOperator::Function f) :
    m_func(std::move(f))
{

}

std::any Conveyor::FuncOperator::execute(const std::any& arg)
{
    return m_func(arg);
}
