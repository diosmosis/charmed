///////////////////////////////////////////////////////////////////////////////
//
/// \file composite_hook.hpp
/// Contains the <c>composite_hook\<\></c> hook type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP

#include <boost/mpl/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/mpl.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    namespace detail
    {
        template <typename IndexSpecifier>
        struct hook_type_of
        {
            typedef typename IndexSpecifier::hook_type type;
        };
    }

    /// \brief A hook type that holds every hook needed by every index of a <c>multi_index_container\<\></c>
    ///        in a <c>fusion::vector\<\></c>.
    ///
    /// \remarks <c>composite_hook\<\></c> derives from the <c>fusion::vector\<\></c> of hooks.
    ///
    /// \tparam IndexSpecifierList the MPL sequence of each index specifier whose hook is to be stored. This
    ///                            can, and in most cases, will, be the same type used in the specific
    ///                            <c>multi_index_container\<\></c> type.
    template <typename IndexSpecifierList>
    struct composite_hook
        : fusion::result_of::as_vector<
            typename mpl::transform<
                IndexSpecifierList,
                detail::hook_type_of<mpl::_1>
            >::type
        >::type
    {};
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )