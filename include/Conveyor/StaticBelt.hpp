#pragma once

#include <type_traits>
#include <tuple>
#include <functional>
#include <utility>

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

        using ActionsTuple = std::tuple<T...>;

        /**
         * @brief Structure for identifying
         * last operator return type.
         * It's deducing type from end to begin.
         * So if deducing fails, operators types are not
         * match.
         * @tparam Index Current operator index.
         * @tparam TupleType Tuple with operators.
         * @tparam FirstArgument First operator argument (for recursion finish).
         */
        template<int Index, typename FirstArgument>
        struct LastElementType
        {
            using element_type = typename std::tuple_element<Index, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().execute(
                    std::declval<typename LastElementType<
                        Index - 1,
                        FirstArgument
                    >::type>()
                )
            );
        };

        template<typename FirstArgument>
        struct LastElementType<0, FirstArgument>
        {
            using element_type = typename std::tuple_element<0, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().execute(
                    std::declval<FirstArgument>()
                )
            );
        };

    public:

        /**
         * @brief Constructor for operators, that's
         * not default constructable.
         * @tparam ConstructorArgs
         * @param args Operator constructors arguments.
         */
        template<typename... ConstructorArgs>
        explicit StaticBelt(ConstructorArgs&&... args) :
            m_actions(std::make_tuple(std::forward<ConstructorArgs>(args)...))
        {

        }

        /**
         * @brief Default.
         */
        StaticBelt<>() = default;

        /**
         * @brief Main execution method.
         * @tparam FirstArg First argument type.
         * @param argument First argument value. Passing by value,
         * to allow user to use std::move to pass argument.
         * @return Last belt executor result.
         */
        template<typename FirstArg>
        typename LastElementType<
            std::tuple_size<ActionsTuple>::value - 1,
            FirstArg
        >::type execute(FirstArg argument)
        {
            return exec_tuple<0, FirstArg, FirstArg>{}(m_actions, std::move(argument));
        }

        /**
         * @brief Method for getting number of
         * operators.
         * @return Number of operators in belt.
         */
        constexpr std::size_t numberOfOperators() const
        {
            return std::tuple_size<ActionsTuple>::value;
        }

        /**
         * @brief Constant method for getting operator
         * by index.
         * @tparam Index Template index value.
         * @return Operator.
         */
        template<std::size_t Index>
        constexpr typename std::tuple_element<Index, ActionsTuple>::type at() const
        {
            return std::get<Index>(m_actions);
        }

        /**
         * @brief Method for getting operator by index.
         * @tparam Index Template index value.
         * @return Operator.
         */
        template<std::size_t Index>
        constexpr typename std::tuple_element<Index, ActionsTuple>::type& at()
        {
            return std::get<Index>(m_actions);
        }

    private:

        /**
         * @brief Structure for iterating through
         * tuple to execute operators.
         * @tparam Index Current operator index.
         * @tparam FirstArgument First argument type.
         * @tparam PreviousArgument Previous argument type.
         */
        template<int Index, typename FirstArgument, typename PreviousArgument>
        struct exec_tuple
        {
            using CurrentElementType = typename std::tuple_element<Index, ActionsTuple>::type;

            typename LastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                FirstArgument
            >::type operator() (ActionsTuple& t, PreviousArgument arg)
            {
                return exec_tuple<
                    Index + 1,
                    FirstArgument,
                    decltype(
                        std::declval<CurrentElementType>().execute(
                            std::declval<PreviousArgument>()
                        )
                    )
                >{}(
                    t,
                    std::get<Index>(t).execute(std::move(arg))
                );
            }
        };

        /**
         * @brief Structure for iterating last
         * tuple to execute operators.
         * @tparam FirstArgument First argument type.
         * @tparam PreviousArgument Previous argument type.
         */
        template<typename FirstArgument, typename PreviousArgument>
        struct exec_tuple<
            std::tuple_size<ActionsTuple>::value - 1,
            FirstArgument,
            PreviousArgument
        >
        {
            typename LastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                FirstArgument
            >::type  operator()(ActionsTuple& t, PreviousArgument arg)
            {
                return std::get<std::tuple_size<ActionsTuple>::value - 1>(t).execute(std::move(arg));
            }
        };

        ActionsTuple m_actions;
    };
}
