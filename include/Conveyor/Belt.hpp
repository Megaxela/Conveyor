#pragma once


#include <vector>
#include "Operator.hpp"

namespace Conveyor
{
    class Belt;

    using BeltPtr = std::shared_ptr<Belt>;

    /**
     * @brief Class, that describes
     * main conveyor belt. Belt is
     * implemented through Operator class,
     * because it can be used as operator.
     */
    class Belt : public Operator
    {
    public:

        using OperatorContainer = std::vector<OperatorPtr>;

        /**
         * @brief Constructor.
         */
        Belt();

        /**
         * @brief Initializer constructor.
         * @param operators Operators.
         */
        Belt(std::initializer_list<OperatorPtr> operators);

        /**
         * @brief Get number of operators in belt.
         * @return Number of operators in belt.
         */
        OperatorContainer::size_type numberOfOperators() const;

        /**
         * @brief Method for getting operator from belt.
         * std::invalid_index exception will be thrown
         * if index is out of range.
         * @param index Operator index.
         * @return Pointer to operator.
         */
        OperatorPtr at(OperatorContainer::size_type index) const;

        /**
         * @brief Method for adding operator to belt.
         * @param o Operator.
         */
        void addOperator(OperatorPtr o);

        /**
         * @brief Method for removing operator from belt.
         * @param o Operator to remove.
         */
        void removeOperator(OperatorPtr o);

        /**
         * @brief Method for removeing operator from belt.
         * @param i Operator index.
         */
        void removeOperator(OperatorContainer::size_type i);

        /**
         * @brief Method for executing belt.
         * @param arg Argument.
         * @return Execution result.
         */
        std::any execute(const std::any& arg) override;

    private:
        OperatorContainer m_operators;
    };
}

