//
// Created by megaxela on 12/1/17.
//

#pragma once

#include <memory>
#include <any>

namespace Conveyor
{
    class Operator;

    using OperatorPtr = std::shared_ptr<Operator>;

    /**
     * @brief Conveyor operator with
     * some job to done.
     */
    class Operator
    {
    public:

        virtual ~Operator() = default;

        /**
         * @brief Method for executing operator job.
         * @param arg Argument from previous operator.
         * @return Operator result.
         */
        virtual std::any execute(const std::any &arg) = 0;

        /**
         * @brief Method for executing operator job.
         * @param arg Argument from previous operator
         * @return Operator result.
         */
        std::any operator()(const std::any& arg)
        {
            return execute(arg);
        }
    };
}

