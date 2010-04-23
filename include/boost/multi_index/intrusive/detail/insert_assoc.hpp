///////////////////////////////////////////////////////////////////////////////
//
/// \file insert_assoc.hpp
/// Contains the insert_assoc\<\> function object.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP

#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>
#include <boost/mpl/at.hpp>
#include <boost/fusion/include/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief A function object that is used by the free functions in
    ///        \ref core_operations.hpp to reinsert modified values into
    ///        the associative indices of a <c>multi_index_container</c>.
    /// 
    /// <c>insert_assoc</c> accepts either a non-const reference to an index
    /// and a reference to a pointer of that index type, or a fusion sequence
    /// with those elements. Sequenced indices are ignored and insertion will
    /// only be attempted on indices if past insertions have succeeded. In case
    /// of a failed insertion, the <c>result</c> member is set to false and the
    /// <c>last_failed</c> member will be set to the index of the of index
    /// whose insertion failed.
    template <typename Value>
    struct insert_assoc
    {
        insert_assoc(Value & v)
            : value(v)
            , result(true)
            , last_failed(-1)
        {}

        template <typename Index>
        typename enable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & idx) const
        {
            if (result)
            {
                result = idx.impl().insert(value).second;

                ++last_failed;
            }
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index &) const
        {
            // empty
        }

        Value & value;
        bool & result;
        mutable unsigned int last_failed; // TODO: This needs a better name.
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP )
