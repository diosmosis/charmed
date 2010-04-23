///////////////////////////////////////////////////////////////////////////////
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
    template <typename Value>
    struct handle_failed_insert
    {
        handle_failed_insert(Value & v, unsigned int lf)
            : value(v)
            , last_failed(lf)
        {}

        template <typename Index>
        void operator()(Index & x) const
        {
            if (last_failed != 0)
            {
                x.impl().erase(x.impl().iterator_to(value));

                --last_failed;
            }
        }

        Value & value;
        mutable unsigned int last_failed;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HANDLE_FAILED_INSERT_HPP )