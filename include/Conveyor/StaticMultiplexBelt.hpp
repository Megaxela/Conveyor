#pragma once

#include <tuple>

namespace Conveyor
{
    /**
     * @brief Compile time defined multiplex belt.
     * @tparam T Types of operators. Operator must
     * have template function `executeForward` with one template
     * `int` argument, that can be specified for different
     * behaviour. Or/And `executeBackward` with same rules.
     */
    template<typename... T>
    class StaticMultiplexBelt
    {

        using ActionsTuple = std::tuple<T...>;

        /**
         * @brief Template structure for identifying
         * last operator return type.
         * It's deducing type from end to begin.
         * (It's required because of function overload)
         * So if deducing fails, operators types are not
         * match.
         * @tparam Index Current operator index in tuple.
         * @tparam FunctionType Function type index.
         * @tparam TupleType Tuple type with operators.
         * @tparam FirstArgument First operator argument (for recursion finish)
         */
        template<int Index, int FunctionType, typename FirstArgument>
        struct ForwardLastElementType
        {
            using element_type = typename std::tuple_element<Index, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().template executeForward<FunctionType>(
                    std::declval<typename ForwardLastElementType<
                        Index - 1,
                        FunctionType,
                        FirstArgument
                    >::type>()
                )
            );
        };

        template<int FunctionType, typename FirstArgument>
        struct ForwardLastElementType<0, FunctionType, FirstArgument>
        {
            using element_type = typename std::tuple_element<0, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().template executeForward<FunctionType>(
                    std::declval<FirstArgument>()
                )
            );
        };

        /**
         * @brief Template structure for identifying
         * first operator return type.
         * It's deducing type from begin to end.
         * (It's required because of function overload)
         * So if deducing fails, operators types are not
         * match.
         * @tparam Index Current operator index in tuple.
         * @tparam FunctionType Function type index.
         * @tparam TupleType Tuple type with operators.
         * @tparam FirstArgument First operator argument (for recursion finish)
         */
        template<int Index, int FunctionType, typename FirstArgument>
        struct BackwardLastElementType
        {
            using element_type = typename std::tuple_element<Index, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().template executeBackward<FunctionType>(
                    std::declval<typename BackwardLastElementType<
                        Index + 1,
                        FunctionType,
                        FirstArgument
                    >::type>()
                )
            );
        };

        template<int FunctionType, typename FirstArgument>
        struct BackwardLastElementType<
            std::tuple_size<ActionsTuple>::value - 1,
            FunctionType,
            FirstArgument
        >
        {
            using element_type = typename std::tuple_element<std::tuple_size<ActionsTuple>::value - 1, ActionsTuple>::type;

            using type = decltype(
                std::declval<element_type>().template executeBackward<FunctionType>(
                    std::declval<FirstArgument>()
                )
            );
        };

        using LastAction = typename std::tuple_element< // Getting last tuple element
            std::tuple_size<ActionsTuple>::value - 1,
            ActionsTuple
        >::type;

        using FirstAction = typename std::tuple_element<
            0,
            ActionsTuple
        >::type;


    public:

        /**
         * @brief Constructor for operators, that's not
         * default constructable.
         * @tparam ConstructorArgs
         * @param args Operator constructors arguments.
         */
        template<typename... ConstructorArgs>
        explicit StaticMultiplexBelt(ConstructorArgs&&... args) :
            m_actions(std::make_tuple(std::forward<ConstructorArgs>(args)...))
        {}

        /**
         * @brief Default constructor.
         */
        StaticMultiplexBelt<>() = default;

        template<int FunctionType, typename FirstArg>
        typename ForwardLastElementType<
            std::tuple_size<ActionsTuple>::value - 1,
            FunctionType,
            FirstArg
        >::type executeForward(FirstArg argument)
        {
            return ForwardExecuteTuple<
                0,
                FunctionType,
                FirstArg,
                FirstArg
            > {}(
                m_actions, std::move(argument)
            );
        }

        template<int FunctionType, typename FirstArg>
        typename BackwardLastElementType<
            0,
            FunctionType,
            FirstArg
        >::type executeBackward(FirstArg argument)
        {
            return BackwardExecuteTuple<
                std::tuple_size<ActionsTuple>::value - 1,
                FunctionType,
                FirstArg,
                FirstArg
            >{}(
                m_actions,
                std::move(argument)
            );

        };

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
        };

        template<std::size_t Index>
        constexpr typename std::tuple_element<Index, ActionsTuple>::type& at()
        {
            return std::get<Index>(m_actions);
        };

    private:

        /**
         * @brief Structure for forward iterating
         * through tuple to execute operators.
         * @tparam Index Current operator index.
         * @tparam FirstArgument First argument type.
         * @tparam PreviousArgument Previous argument type.
         */
        template<int Index, int FunctionType, typename FirstArgument, typename PreviousArgument>
        struct ForwardExecuteTuple
        {
            using CurrentElementType =
                typename std::tuple_element<Index, ActionsTuple>::type;

            typename ForwardLastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                FunctionType,
                FirstArgument
            >::type operator() (ActionsTuple& t, PreviousArgument arg)
            {
                return ForwardExecuteTuple<
                    Index + 1,
                    FunctionType,
                    FirstArgument,
                    decltype(
                        std::declval<CurrentElementType>().template executeForward<FunctionType>(
                            std::declval<PreviousArgument>()
                        )
                    )
                >{}(
                    t,
                    std::get<Index>(t).template executeForward<FunctionType>(std::move(arg))
                );
            };
        };

        template<int FunctionType, typename FirstArgument, typename PreviousArgument>
        struct ForwardExecuteTuple<
            std::tuple_size<ActionsTuple>::value - 1,
            FunctionType,
            FirstArgument,
            PreviousArgument
        >
        {
            typename ForwardLastElementType<
                std::tuple_size<ActionsTuple>::value - 1,
                FunctionType,
                FirstArgument
            >::type operator()(ActionsTuple& t, PreviousArgument arg)
            {
                return std::get<std::tuple_size<ActionsTuple>::value - 1>(t).template executeForward<FunctionType>(std::move(arg));
            }
        };

        /**
         * @brief Structure for backward iterating
         * through tuple to execute operators.
         * @tparam Index Current operator index.
         * @tparam FirstArgument First argument type.
         * @tparam PreviousArgument Previous argument type.
         */
        template<int Index, int FunctionType, typename FirstArgument, typename PreviousArgument>
        struct BackwardExecuteTuple
        {
            using CurrentElementType =
                typename std::tuple_element<Index, ActionsTuple>::type;

            typename BackwardLastElementType<
                0,
                FunctionType,
                FirstArgument
            >::type operator()(ActionsTuple& t, PreviousArgument arg)
            {
                return BackwardExecuteTuple<
                    Index - 1,
                    FunctionType,
                    FirstArgument,
                    decltype(
                        std::declval<CurrentElementType>().template executeBackward<FunctionType>(
                            std::declval<PreviousArgument>()
                        )
                    )
                >{}(
                    t,
                    std::get<Index>(t).template executeBackward<FunctionType>(std::move(arg))
                );
            }
        };

        template<int FunctionType, typename FirstArgument, typename PreviousArgument>
        struct BackwardExecuteTuple<0, FunctionType, FirstArgument, PreviousArgument>
        {
            typename BackwardLastElementType<
                0,
                FunctionType,
                FirstArgument
            >::type operator()(ActionsTuple& t, PreviousArgument arg)
            {
                return std::get<0>(t).template executeBackward<FunctionType>(std::move(arg));
            }
        };

        ActionsTuple m_actions;

    };
}