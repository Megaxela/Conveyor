//
// Created by megaxela on 12/1/17.
//

#include <algorithm>
#include "Conveyor/Belt.hpp"

Conveyor::Belt::Belt() :
    m_operators()
{

}

Conveyor::Belt::Belt(std::initializer_list<Conveyor::OperatorPtr> operators) :
    m_operators(operators)
{

}

Conveyor::Belt::OperatorContainer::size_type Conveyor::Belt::numberOfOperators() const
{
    return m_operators.size();
}

Conveyor::OperatorPtr Conveyor::Belt::at(OperatorContainer::size_type index) const
{
    return m_operators.at(index);
}

void Conveyor::Belt::addOperator(Conveyor::OperatorPtr o)
{
    m_operators.emplace_back(std::move(o));
}

void Conveyor::Belt::removeOperator(Conveyor::OperatorPtr o)
{
    m_operators.erase(
        std::find(
            m_operators.begin(),
            m_operators.end(),
            o
        ),
        m_operators.end()
    );
}

void Conveyor::Belt::removeOperator(OperatorContainer::size_type i)
{
    m_operators.erase(m_operators.begin() + i);
}

std::any Conveyor::Belt::execute(const std::any& arg)
{
    auto argument = arg;
    for (auto&& operatorObject : m_operators)
    {
        argument = operatorObject->execute(argument);
    }

    return argument;
}
