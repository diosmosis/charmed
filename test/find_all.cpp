///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_find_all
#include <boost/test/included/unit_test.hpp>
#include <charmed/query/find_all.hpp>
#include <algorithm>
#include "test_attributes.hpp"

using namespace charmed;

BOOST_AUTO_TEST_CASE( find_all_test )
{
    typedef metadata_range<my_attribute, 2>::type result_type;

    {
        result_type attrs = query::find_all<my_attribute, 2>(2);

        BOOST_CHECK(std::distance(attrs.begin(), attrs.end()) == 3);

        for (result_type::iterator i = attrs.begin(); i != attrs.end(); ++i)
        {
            BOOST_CHECK(i->int_key == 2);
        }
    }

    {
        result_type attrs = query::find_all<my_attribute, 2>(4);

        BOOST_CHECK(std::distance(attrs.begin(), attrs.end()) == 1);

        BOOST_CHECK(attrs.begin()->int_key == 4);
    }
}
