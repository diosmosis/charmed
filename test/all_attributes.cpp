///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_all_attributes
#include <boost/test/included/unit_test.hpp>
#include <charmed/query/all_attributes.hpp>
#include "test_attributes.hpp"

using namespace charmed;

template <typename Range>
bool key_is_in_attribute_range(Range const& rng, std::string const& key)
{
    for (typename Range::const_iterator i = rng.begin(); i != rng.end(); ++i)
    {
        if (i->key == key)
        {
            return true;
        }
    }

    return false;
}

BOOST_AUTO_TEST_CASE( all_attributes_sequenced_test )
{
    typedef query::result_of::all_attributes<my_attribute, 0>::type attribute_range;

    attribute_range attrs = query::all_attributes<my_attribute>();

    BOOST_CHECK(key_is_in_attribute_range(attrs, "a"));
    BOOST_CHECK(key_is_in_attribute_range(attrs, "b"));
    BOOST_CHECK(key_is_in_attribute_range(attrs, "c"));
    BOOST_CHECK(key_is_in_attribute_range(attrs, "d"));
}

BOOST_AUTO_TEST_CASE( all_attributes_ordered_test )
{
    typedef query::result_of::all_attributes<my_attribute, 1>::type attribute_range;

    attribute_range attrs = query::all_attributes<my_attribute, 1>();

    attribute_range::iterator i = attrs.begin();

    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->key == "a");

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->key == "b");

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->key == "c");

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->key == "d");

    ++i;
    BOOST_CHECK(i == attrs.end());
}
