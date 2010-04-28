///////////////////////////////////////////////////////////////////////////////
//
/// \file handle_failed_insert.hpp
/// Contains the <c>handle_failed_insert\<\></c> function object type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HANDLE_FAILED_INSERT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HANDLE_FAILED_INSERT_HPP

// TODO: clean up includes in all files.
namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief A function object used by the \ref core_operations.hpp to erase a value
    ///        from indices when insertion into another one fails.
    ///
    /// When insertion is requested, the value to be inserted must be inserted into
    /// every index. If for some reason insertion fails in one of the other indices,
    /// <c>handle_failed_insert\<\></c> is used to remove the value from the indices
    /// before it in the index tuple.
    template <typename Value>
    struct handle_failed_insert
    {
        handle_failed_insert(Value & v, unsigned int ic)
            : value(v)
            , index_count(ic)
        {}

        template <typename Index>
        void operator()(Index & x) const
        {
            // TODO: don't need this check if I use boost::switch
            if (index_count != 0)
            {
                x.erase(x.iterator_to(value));

                --index_count;
            }
        }

        Value & value;
        mutable unsigned int index_count;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HANDLE_FAILED_INSERT_HPP )