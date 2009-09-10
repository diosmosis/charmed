///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_TAG_FUNCTION_HPP )
#define CHARMED_TAG_FUNCTION_HPP

#include <charmed/detail/metadata_initializer.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/typeof/typeof.hpp>

#define CHARMED_TAG_FUNCTION(f, expr)                                                                                 \
    charmed::detail::metadata_initializer<BOOST_TYPEOF(expr)>                                                         \
        charmed::function_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f)>::type<f>::metadata(f, expr)

namespace charmed
{
    // TODO use preprocessor iteration
    template <typename M, typename F>
    struct function_association;

    template <typename M, typename R>
    struct function_association<M, R(*)()>
    {
        template <R(*F)()>
        struct type
        {
            static detail::metadata_initializer<M> metadata;
        };
    };

    template <typename M, typename R, typename A0>
    struct function_association<M, R(*)(A0)>
    {
        template <R(*F)(A0)>
        struct type
        {
            static detail::metadata_initializer<M> metadata;
        };
    };
}

#endif // #if !defined( CHARMED_TAG_FUNCTION_HPP )