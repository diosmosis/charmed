///////////////////////////////////////////////////////////////////////////////
//
/// \file erase_assoc.hpp
/// Contains the <c>erase_assoc\<\></c> function object type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP

#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief Used by \ref core_operations.hpp to erase a value from every associative index
    ///        of an intrusive <c>multi_index_container</c>.
    template <typename Value>
    struct erase_assoc
    {
        erase_assoc(Value & v)
            : value(v)
        {}

        template <typename Index>
        typename enable_if<
            is_associative_container<Index>, void
        >::type operator()(Index & x) const
        {
            x.erase(x.iterator_to(value));
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<Index>, void
        >::type operator()(Index &) const
        {
            // empty
        }

        Value & value;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP )