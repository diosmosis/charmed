///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CHARMED_TAG_PARAMETER_HPP )
#define CHARMED_TAG_PARAMETER_HPP

#include <charmed/detail/metadata_initializer.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/static_assert.hpp>

#include <utility>

#define CHARMED_TAG_PARAMETER(f, i, expr)                                                                             \
    std::pair<void *, int>                                                                                            \
        charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::runtime_type_data(f, i);     \
    charmed::detail::metadata_initializer<BOOST_TYPEOF(expr)>                                                         \
        charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::metadata(                    \
            &charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::runtime_type_data, expr)

namespace charmed
{
    template <typename M, typename F, int N>
    struct parameter_association;

#define CHARMED_n BOOST_PP_ITERATION()
#define BOOST_PP_FILENAME_1 <charmed/tag/parameter.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, CHARMED_PARAMETER_LIMIT)
#include BOOST_PP_ITERATE()
#undef CHARMED_n
}

#endif // #if !defined( CHARMED_TAG_PARAMETER_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename M, typename R BOOST_PP_ENUM_TRAILING_PARAMS(CHARMED_n, typename A), int N>
    struct parameter_association<M, R(*)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A)), N>
    {
        BOOST_STATIC_ASSERT(N >= 0 && N < CHARMED_n);

        template <R(*F)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A))>
        struct type
        {
            static std::pair<void *, int> runtime_type_data;
            static detail::metadata_initializer<M> metadata;
        };
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
