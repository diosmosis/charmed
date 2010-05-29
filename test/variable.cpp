///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_variable
#include <boost/test/included/unit_test.hpp>
#include <charmed/tag/variable.hpp>
#include <charmed/query/metadata_of.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>

using namespace charmed;

static int my_var = 0;

namespace my_ns
{
    static float my_other_var = 23;
}

struct variable_name_attribute
{
    variable_name_attribute(std::string const& n)
        : name(n)
    {}

    std::string name;
};

CHARMED_TAG_VARIABLE(my_var, 0, variable_name_attribute("my_var"));
CHARMED_TAG_VARIABLE(my_ns::my_other_var, 1, variable_name_attribute("my_other_var"));

BOOST_AUTO_TEST_CASE( metadata_of_test )
{
    BOOST_REQUIRE(query::metadata_of<variable_name_attribute>(&my_var));
    BOOST_CHECK(query::metadata_of<variable_name_attribute>(&my_var)->name == "my_var");
    BOOST_CHECK(query::tagged_type_of<int>(
        *query::metadata_of<variable_name_attribute>(&my_var)) == &my_var);

    BOOST_REQUIRE(query::metadata_of<variable_name_attribute>(&my_ns::my_other_var));
    BOOST_CHECK(query::metadata_of<variable_name_attribute>(&my_ns::my_other_var)->name == "my_other_var");
    BOOST_CHECK(query::tagged_type_of<float>(
        *query::metadata_of<variable_name_attribute>(&my_ns::my_other_var)) == &my_ns::my_other_var);
}
