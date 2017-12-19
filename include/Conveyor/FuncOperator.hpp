//
// Created by megaxela on 12/1/17.
//

#pragma once

#include "Operator.hpp"
#include <functional>
#include <any>

namespace Conveyor
{
    class FuncOperator;

    using FuncOperatorPtr = std::shared_ptr<FuncOperator>;

    class FuncOperator : public Operator
    {
    public:
        using Function = std::function<std::any(const std::any& val)>;

        /**
         * @brief Constructor from lambda.
         * @param f Function.
         */
        explicit FuncOperator(Function f);

        /**
         * @brief Method for executing lambda.
         * @param arg Argument.
         * @return Lambda result.
         */
        std::any execute(const std::any& arg) override;

    private:
        Function m_func;
    };
}

