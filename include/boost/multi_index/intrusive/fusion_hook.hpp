///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_FUSION_HOOK_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_FUSION_HOOK_HPP

#include <boost/multi_index/intrusive/detail/vector_from_element.hpp>

#include <boost/intrusive/detail/parent_from_member.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/link_mode.hpp>

#include <boost/fusion/include/at_c.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    template <typename T, typename HookTuple, int N, HookTuple T::* P>
    struct fusion_hook_traits
    {
        typedef typename mpl::at_c<HookTuple, N>::type                      hook_type;
        typedef typename hook_type::boost_intrusive_tags::node_traits       node_traits;
        typedef typename node_traits::node                                  node;
        typedef typename node_traits::node_ptr                              node_ptr;
        typedef typename node_traits::const_node_ptr                        const_node_ptr;
        typedef T                                                           value_type;
        typedef typename boost::pointer_to_other<node_ptr, T>::type         pointer;
        typedef typename boost::pointer_to_other<node_ptr, const T>::type   const_pointer;
        typedef typename std::iterator_traits<pointer>::reference           reference;
        typedef typename std::iterator_traits<const_pointer>::reference     const_reference;

        static boost::intrusive::link_mode_type const link_mode = hook_type::boost_intrusive_tags::link_mode;

        static node_ptr to_node_ptr(reference value)
        {
            return static_cast<node *>(&fusion::at_c<N>(value.*P));
        }

        static const_node_ptr to_node_ptr(const_reference value)
        {
            return static_cast<node const*>(&fusion::at_c<N>(value.*P));
        }

        static pointer to_value_ptr(node_ptr n)
        {
            hook_type * hook = static_cast<hook_type *>(boost::intrusive::detail::boost_intrusive_get_pointer(n));
            HookTuple * hook_tuple = detail::vector_from_element<HookTuple, N>(hook);
            return boost::intrusive::detail::parent_from_member<T, HookTuple>(hook_tuple, P);
        }

        static const_pointer to_value_ptr(const_node_ptr n)
        {
            hook_type const* hook = static_cast<hook_type const*>(
                boost::intrusive::detail::boost_intrusive_get_pointer(n));
            HookTuple const* hook_tuple = detail::vector_from_element<HookTuple, N>(hook);
            return boost::intrusive::detail::parent_from_member<T, HookTuple>(hook_tuple, P);
        }
    };

    template <typename T, typename HookTuple, int N, HookTuple T::* P>
    struct fusion_hook
    {
        typedef fusion_hook_traits<T, HookTuple, N, P> fusion_value_traits;

        template <typename Base>
        struct pack
            : Base
        {
            typedef fusion_value_traits value_traits;
        };
    };

    template <typename T, typename HookTuple, HookTuple T::* P>
    struct fusion_hook_specifier
    {
        template <int N>
        struct apply
        {
            typedef fusion_hook<T, HookTuple, N, P> type;
        };
    };
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_FUSION_HOOK_HPP )
