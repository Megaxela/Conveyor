//
// Created by megaxela on 12/19/17.
//

#pragma once

#include <type_traits>
#include <tuple>
#include <functional>

namespace Conveyor
{
    /**
     * @brief Compile time defined belt.
     * @tparam T Types of operators. Operator
     * must have `execute` method.
     */
    template<typename... T>
    class StaticBelt
    {

        template<int Index, typename TupleType, typename FirstArgument>
        struct LastElementType
        {
            using element_type = typename std::tuple_element<Index, TupleType>::type;

            using type = typename std::result_of<
                decltype(&element_type::execute)(
                    element_type,
                    typename LastElementType<Index - 1, TupleType, FirstArgument>::type
                )
            >::type;
        };

        template<typename TupleType, typename FirstArgument>
        struct LastElementType<0, TupleType, FirstArgument>
        {
            using element_type = typename std::tuple_element<0, TupleType>::type;

            using type = typename std::result_of<
                decltype(&element_type::execute)(element_type, FirstArgument)
            >::type;
        };

        using ActionsTuple = std::tuple<T...>;

        using LastAction = typename std::tuple_element< // Getting last tuple element
            std::tuple_size<ActionsTuple>::value - 1,
            ActionsTuple
        >::type;

        using FirstAction = typename std::tuple_element<
            0,
            ActionsTuple
        >::type;

    public:

        template<typename... ConstructorArgs>
        StaticBelt(ConstructorArgs&&... args) :
            m_actions(std::make_tuple(args...))
        {

        }

        StaticBelt<>()
        {

        }

        template<typename FirstArg>
        typename LastElementType<
            std::tuple_size<ActionsTuple>::value - 1,
            ActionsTuple,
            FirstArg
        >::type execute(const FirstArg& argument)
        {
            return exec<FirstArg>(m_actions, argument);
        }

        constexpr std::size_t numberOfOperators() const
        {
            return std::tuple_size<ActionsTuple>::value;
        }

        template<std::size_t Index>
        constexpr typename std::tuple_element<Index, ActionsTuple>::type at() const
        {
            return std::get<Index>(m_actions);
        }

        template<std::size_t Index>
        constexpr typename std::tuple_element<Index, ActionsTuple>::type& at()
        {
            return std::get<Index>(m_actions);
        }

    private:

        template<int Index, typename FirstArgument, typename PreviousArgument>
        struct exec_tuple
        {
            using CurrentElementType = typename std::tuple_element<Index, ActionsTuple>::type;

            typename LastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                ActionsTuple,
                FirstArgument
            >::type operator() (ActionsTuple& t, const PreviousArgument& arg)
            {
                return exec_tuple<
                    Index + 1,
                    FirstArgument,
                    typename std::result_of<
                        decltype(&CurrentElementType::execute)(CurrentElementType, PreviousArgument)
                    >::type
                >{}(
                    t,
                    std::get<Index>(t).execute(arg)
                );
            }
        };

        template<typename FirstArgument, typename PreviousArgument>
        struct exec_tuple<
            std::tuple_size<ActionsTuple>::value - 1,
            FirstArgument,
            PreviousArgument
        >
        {
            typename LastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                ActionsTuple,
                FirstArgument
            >::type  operator()(ActionsTuple& t, const PreviousArgument& arg)
            {
                return std::get<std::tuple_size<ActionsTuple>::value - 1>(t).execute(arg);
            }
        };

        template<typename FirstArgument>
        typename LastElementType<
            std::tuple_size<ActionsTuple>::value - 1,
            ActionsTuple,
            FirstArgument
        >::type exec(ActionsTuple& a, const FirstArgument& argument)
        {
            return exec_tuple<0, FirstArgument, FirstArgument>{}(a, argument);
        }

        ActionsTuple m_actions;
    };
}
