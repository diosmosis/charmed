///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_SEQUENCED_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_SEQUENCED_INDEX_HPP

#include <boost/multi_index/intrusive/detail/common_index.hpp>
#include <boost/multi_index/intrusive/index_fwd.hpp>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    template <typename MultiIndex, typename Impl, typename Specifier>
    struct sequenced_index : detail::common_index<MultiIndex, Impl, Specifier>
    {
        typedef detail::common_index<MultiIndex, Impl, Specifier> base_type;

        typedef typename impl_type::reverse_iterator            reverse_iterator;
        typedef typename impl_type::const_reverse_iterator      const_reverse_iterator;

        sequenced_index(multi_index_type & x, Impl & i)
            : base_type(x, i)
        {}

        reverse_iterator rbegin()
        {
            return impl().rbegin();
        }

        reverse_iterator rend()
        {
            return impl().rend();
        }

        const_reverse_iterator rend() const
        {
            return impl().rend();
        }

        const_reverse_iterator rbegin() const
        {
            return impl().rbegin();
        }

        std::pair<iterator, bool> push_front(value_type & x)
        {
            return insert(begin(), x);
        }

        void pop_front()
        {
            erase(begin());
        }

        std::pair<iterator, bool> push_back(value_type & x)
        {
            return insert(end(), x);
        }

        void pop_back()
        {
            erase(--end());
        }

        std::pair<iterator, bool> insert(iterator position, value_type & x)
        {
            return detail::insert_sequenced(container, *this, position, x);
        }

        reference front()
        {
            return impl().front();
        }

        const_reference front() const
        {
            return impl().front();
        }

        reference back()
        {
            return impl().back();
        }

        const_reference back() const
        {
            return impl().back();
        }
    };

    struct sequenced
    {
        typedef boost::intrusive::list_member_hook<> hook_type;

        template <typename Value, typename Hook, int N>
        struct impl_index
        {
            typedef boost::intrusive::list<Value, typename Hook::template apply<N>::type> type;
        };

        template <typename MultiIndex, typename Value, typename Hook, int N>
        struct index_class
        {
            typedef sequenced_index<MultiIndex, typename impl_index<Value, Hook, N>::type, sequenced> type;
        };
    };
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_SEQUENCED_INDEX_HPP )