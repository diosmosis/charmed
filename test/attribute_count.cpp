///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_attribute_count
#include <boost/test/included/unit_test.hpp>
#include <charmed/query/attribute_count.hpp>
#include "test_attributes.hpp"

using namespace charmed;

BOOST_AUTO_TEST_CASE( attribute_count_test )
{
    BOOST_CHECK(query::attribute_count<my_attribute>() == 4);
    BOOST_CHECK((query::attribute_count<my_attribute, 2>(2) == 3));
    BOOST_CHECK((query::attribute_count<my_attribute, 2>(4) == 1));
    BOOST_CHECK((query::attribute_count<my_attribute, 2>(34) == 0));
}
