///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_parameter
#include <boost/test/included/unit_test.hpp>
#include <charmed/tag/parameter.hpp>
#include <charmed/query/parameter.hpp>
#include <charmed/query/all_attributes.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>

using namespace charmed;

void my_function(int arg1, std::string arg2, int whatever, float whatever2)
{
    // empty
}

struct parameter_name_attribute
{
    parameter_name_attribute(std::string const& x)
        : name(x)
    {}

    std::string name;
};

CHARMED_TAG_PARAMETER(&my_function, 0, parameter_name_attribute("arg1"));
CHARMED_TAG_PARAMETER(&my_function, 1, parameter_name_attribute("arg2"));
CHARMED_TAG_PARAMETER(&my_function, 3, parameter_name_attribute("whatever2"));

// TODO: Get this test case to work.
/*
BOOST_AUTO_TEST_CASE( parameter_attribute_ct )
{
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute, 0>(&my_function) == "arg1");
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute, 1>(&my_function) == "arg2");
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute, 2>(&my_function) == "whatever");
}
*/
BOOST_AUTO_TEST_CASE( parameter_attribute_rt_test )
{
    parameter_name_attribute * attr = 0;

    BOOST_REQUIRE(query::parameter_attribute<parameter_name_attribute>(&my_function, 0));
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute>(&my_function, 0)->name == "arg1");

    BOOST_REQUIRE(query::parameter_attribute<parameter_name_attribute>(&my_function, 1));
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute>(&my_function, 1)->name == "arg2");

    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute>(&my_function, 2) == 0);

    BOOST_REQUIRE(query::parameter_attribute<parameter_name_attribute>(&my_function, 3));
    BOOST_CHECK(query::parameter_attribute<parameter_name_attribute>(&my_function, 3)->name == "whatever2");
}

BOOST_AUTO_TEST_CASE( parameter_attributes_test )
{
    typedef query::result_of::parameter_attributes<parameter_name_attribute>::type attribute_range;
    typedef attribute_range::iterator iterator;

    attribute_range attrs = query::parameter_attributes<parameter_name_attribute>(&my_function);

    iterator i = attrs.begin();
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "arg1");

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "arg2");

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "whatever2");

    ++i;
    BOOST_CHECK(i == attrs.end());
}

// TODO: There needs to be a way to refer to a parameter of a function using a void *. So it would be possible to say
// metadata_of(function_parameter(&my_function, 1)) or tagged_type_of(attr) == function_parameter(&my_function, 1).
// won't need parameter_attribute.
BOOST_AUTO_TEST_CASE( all_attributes_test )
{
    typedef query::result_of::all_attributes<parameter_name_attribute>::type attribute_range;
    typedef attribute_range::iterator iterator;

    attribute_range attrs = query::all_attributes<parameter_name_attribute>();

    iterator i = attrs.begin();
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "arg1");
    BOOST_CHECK(query::tagged_type_of_raw(*i) != 0);

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "arg2");
    BOOST_CHECK(query::tagged_type_of_raw(*i) != 0);

    ++i;
    BOOST_REQUIRE(i != attrs.end());
    BOOST_CHECK(i->name == "whatever2");
    BOOST_CHECK(query::tagged_type_of_raw(*i) != 0);

    ++i;
    BOOST_CHECK(i == attrs.end());
}
