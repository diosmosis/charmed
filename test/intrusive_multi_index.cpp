///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

// TODO: Fill this out.
#define BOOST_TEST_MODULE intrusive_multi_index
#include <boost/test/included/unit_test.hpp>
#include <boost/multi_index/intrusive/multi_index_container.hpp>
#include <boost/multi_index/intrusive/ordered_index.hpp>
#include <boost/multi_index/intrusive/unordered_index.hpp>
#include <boost/multi_index/intrusive/sequenced_index.hpp>
#include <boost/multi_index/intrusive/composite_hook.hpp>
#include <boost/multi_index/intrusive/fusion_hook.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost::multi_index;

struct value_type_impl
{
    int ordered_unique_key;
    int ordered_non_unique_key;
    int unordered_unique_key;
    int unordered_non_unique_key;
};

struct value_type : value_type_impl
{
    typedef indexed_by<
        intrusive::sequenced<>,
        intrusive::ordered_unique<member<value_type_impl, int, &value_type_impl::ordered_unique_key> >,
        intrusive::ordered_non_unique<member<value_type_impl, int, &value_type_impl::ordered_non_unique_key> >,
        intrusive::unordered_unique<member<value_type_impl, int, &value_type_impl::unordered_unique_key> >,
        intrusive::unordered_non_unique<member<value_type_impl, int, &value_type_impl::unordered_non_unique_key> >
    > index_specifier;

    typedef intrusive::composite_hook<index_specifier> hook_type;

    hook_type hook;
};

typedef intrusive::multi_index_container<
    value_type,
    value_type::index_specifier,
    intrusive::fusion_hook_specifier<value_type, value_type::hook_type, &value_type::hook>
> container;

BOOST_AUTO_TEST_CASE( container_construct_test )
{
    container c;
}
