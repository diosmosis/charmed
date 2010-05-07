///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_function
#include <boost/test/included/unit_test.hpp>
#include <charmed/tag/function.hpp>
#include <charmed/query/all_attributes.hpp>
#include <charmed/query/metadata_of.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>

using namespace charmed;

void my_function(int argc, char ** argv)
{
    // empty
}

int my_other_function()
{
    return 0;
}

float my_last_function(std::string)
{
    return 0.0f;
}

struct function_name_attribute
{
    function_name_attribute(std::string const& n)
        : name(n)
    {}

    std::string name;
};

CHARMED_TAG_FUNCTION(&my_function, function_name_attribute("my_function"));
CHARMED_TAG_FUNCTION(&my_other_function, function_name_attribute("my_other_function"));
CHARMED_TAG_FUNCTION(&my_last_function, function_name_attribute("my_last_function"));

BOOST_AUTO_TEST_CASE( all_attributes_test )
{
    typedef query::result_of::all_attributes<function_name_attribute>::type attribute_range;
    typedef boost::range_iterator<attribute_range>::type iterator;

    attribute_range all = query::all_attributes<function_name_attribute>();

    iterator i = all.begin();
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "my_function");
    BOOST_CHECK(query::tagged_type_of<void (int, char **)>(*i) == &my_function);

    ++i;
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "my_other_function");
    BOOST_CHECK(query::tagged_type_of<int ()>(*i) == &my_other_function);

    ++i;
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "my_last_function");
    BOOST_CHECK(query::tagged_type_of<float (std::string)>(*i) == &my_last_function);

    ++i;
    BOOST_CHECK(i == all.end());
}

BOOST_AUTO_TEST_CASE( metadata_of_test )
{
    BOOST_REQUIRE(query::metadata_of<function_name_attribute>(&my_function));
    BOOST_CHECK(query::metadata_of<function_name_attribute>(&my_function)->name == "my_function");

    BOOST_REQUIRE(query::metadata_of<function_name_attribute>(&my_other_function));
    BOOST_CHECK(query::metadata_of<function_name_attribute>(&my_other_function)->name == "my_other_function");

    BOOST_REQUIRE(query::metadata_of<function_name_attribute>(&my_last_function));
    BOOST_CHECK(query::metadata_of<function_name_attribute>(&my_last_function)->name == "my_last_function");
}

// TODO: This should be moved to another unit test file.
BOOST_AUTO_TEST_CASE( tagged_type_of_test )
{
    BOOST_REQUIRE(query::metadata_of<function_name_attribute>(&my_function));
    BOOST_CHECK(
        query::tagged_type_of<void (int, char **)>(
            *query::metadata_of<function_name_attribute>(&my_function)) == &my_function);

    try
    {
        query::tagged_type_of<int>(
            *query::metadata_of<function_name_attribute>(&my_function));

        BOOST_CHECK_MESSAGE(false, "succeeded in getting the type associated w/ function metadata as an int");
    }
    catch (charmed::bad_metadata_cast const&)
    {
        // ignore
    }
}
