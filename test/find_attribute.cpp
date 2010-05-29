///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_find_attributes
#include <boost/test/included/unit_test.hpp>
#include <charmed/query/find_attribute.hpp>
#include "test_attributes.hpp"

using namespace charmed;

BOOST_AUTO_TEST_CASE( find_attribute_test )
{
    BOOST_REQUIRE((query::find_attribute<my_attribute, 1>("a")));
    BOOST_CHECK((query::find_attribute<my_attribute, 1>("a")->count == 0));

    BOOST_REQUIRE((query::find_attribute<my_attribute, 1>("c")));
    BOOST_CHECK((query::find_attribute<my_attribute, 1>("c")->count == 1));

    BOOST_REQUIRE((query::find_attribute<my_attribute, 1>("d")));
    BOOST_CHECK((query::find_attribute<my_attribute, 1>("d")->count == 2));

    BOOST_REQUIRE((query::find_attribute<my_attribute, 1>("b")));
    BOOST_CHECK((query::find_attribute<my_attribute, 1>("b")->count == 3));
}
