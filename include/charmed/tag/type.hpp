///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CHARMED_TAG_TYPE_HPP )
#define CHARMED_TAG_TYPE_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/metadata_initializer.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/typeof/typeof.hpp>

#define CHARMED_TAG_TYPE(type, expr)                                                                                  \
    charmed::metadata_initializer<BOOST_TYPEOF(expr)>                                                                 \
        charmed::type_association<type, BOOST_TYPEOF(expr)>::_(&typeid(type), expr)

#define CHARMED_PTA(t, n, expr) charmed::partial_template_association<BOOST_TYPEOF(expr), n>::type<t>

#define CHARMED_TAG_TEMPLATE_TYPE(t, n, expr)                                                                         \
    charmed::metadata_initializer<BOOST_TYPEOF(expr)>                                                                 \
        CHARMED_PTA(t, n, expr)::_(&charmed::partial_typeid_of<n>::get<t>(), expr)

namespace charmed
{
    template <typename T, typename M>
    struct type_association
    {
        static metadata_initializer<M> _;
    };

    template <typename M, int N>
    struct partial_template_association;

    template <int N>
    struct partial_typeid_of;

    #define CHARMED_n BOOST_PP_ITERATION()
    #define BOOST_PP_FILENAME_1 <charmed/tag/type.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CHARMED_PARTIAL_TEMPLATE_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CHARMED_n
}

#endif // #if !defined( CHARMED_TAG_TYPE_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename M>
    struct partial_template_association<M, CHARMED_n>
    {
        template <
            template <BOOST_PP_ENUM_PARAMS(CHARMED_n, typename T)> class T
        >
        struct type
        {
            static metadata_initializer<M> _;
        };
    };

    template <>
    struct partial_typeid_of<CHARMED_n>
    {
        template <
            template <BOOST_PP_ENUM_PARAMS(CHARMED_n, typename T)> class T
        >
        static std::type_info const& get() { return typeid(T); }
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
