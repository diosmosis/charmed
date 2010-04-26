///////////////////////////////////////////////////////////////////////////////
//
/// \file erase_single.hpp
/// Contains the <c>erase_single\<\></c> function object type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief A function object used by \ref core_operations.hpp to erase a single value from
    ///        every index.
    template <typename Value>
    struct erase_single
    {
        erase_single(Value const& v)
            : value(v)
        {}

        template <typename Index>
        void operator()(Index & x) const
        {
            x.impl().erase(x.impl().iterator_to(value));
        }

        Value const& value;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP )