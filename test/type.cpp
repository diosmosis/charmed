///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_type
#include <boost/test/included/unit_test.hpp>
#include <charmed/tag/type.hpp>
#include <charmed/query/metadata_of.hpp>
#include <charmed/query/all_attributes.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>
#include <iostream>
using namespace charmed;

struct my_type
{
};

template <typename A, typename B>
struct my_template_type_0
{
};

template <typename A>
struct my_template_type_1
{
};

typedef void (my_function_type)(int, int, int);

typedef void * my_pointer_type;

typedef int my_type::* my_member_pointer_type;

struct type_name_attribute
{
    type_name_attribute(std::string const& n)
        : name(n)
    {}

    std::string name;
};

CHARMED_TAG_TYPE(my_type, type_name_attribute("my_type"));
CHARMED_TAG_TYPE(my_function_type, type_name_attribute("void (int, int, int)"));
CHARMED_TAG_TYPE(my_pointer_type, type_name_attribute("void *"));
CHARMED_TAG_TYPE(my_member_pointer_type, type_name_attribute("int my_type::*"));

CHARMED_TAG_TEMPLATE_TYPE(my_template_type_0, 2, type_name_attribute("my_template_type_0"));
CHARMED_TAG_TEMPLATE_TYPE(my_template_type_1, 1, type_name_attribute("my_template_type_1"));

BOOST_AUTO_TEST_CASE( metadata_of_test )
{
    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(typeid(my_type)));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(typeid(my_type))->name == "my_type");

    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(typeid(my_function_type)));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(typeid(my_function_type))->name == "void (int, int, int)");

    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(typeid(my_pointer_type)));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(typeid(my_pointer_type))->name == "void *");

    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(typeid(my_member_pointer_type)));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(typeid(my_member_pointer_type))->name == "int my_type::*");

    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(partial_typeid_of<2>::get<my_template_type_0>()));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(partial_typeid_of<2>::get<my_template_type_0>())->name == "my_template_type_0");

    BOOST_REQUIRE(query::metadata_of<type_name_attribute>(partial_typeid_of<1>::get<my_template_type_1>()));
    BOOST_CHECK(query::metadata_of<type_name_attribute>(partial_typeid_of<1>::get<my_template_type_1>())->name == "my_template_type_1");
}

// TODO: Should I have this test case here? Should it expect attributes to be in the order they were added?
// NOTE: This shouldn't be here. all_attributes should have its own test suite
BOOST_AUTO_TEST_CASE( all_attributes_test )
{
    typedef query::result_of::all_attributes<type_name_attribute>::type attribute_range;
    typedef boost::range_iterator<attribute_range>::type iterator;

    attribute_range all = query::all_attributes<type_name_attribute>();

    iterator i = all.begin();
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "my_type");
    BOOST_CHECK(*query::tagged_type_of<std::type_info>(*i) == typeid(my_type));

    ++i;
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "void (int, int, int)");
    BOOST_CHECK(*query::tagged_type_of<std::type_info>(*i) == typeid(my_function_type));

    ++i;
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "void *");
    BOOST_CHECK(*query::tagged_type_of<std::type_info>(*i) == typeid(my_pointer_type));

    ++i;
    BOOST_REQUIRE(i != all.end());
    BOOST_CHECK(i->name == "int my_type::*");
    BOOST_CHECK(*query::tagged_type_of<std::type_info>(*i) == typeid(my_member_pointer_type));

//    ++i;
//    BOOST_CHECK(i == all.end());
}
