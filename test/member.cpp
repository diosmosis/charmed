///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE charmed_member
#include <boost/test/included/unit_test.hpp>
#include <charmed/tag/member.hpp>
#include <charmed/query/metadata_of.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>

using namespace charmed;

struct my_type
{
    int member1;
    my_type * member2;
};

struct member_name_attribute
{
    member_name_attribute(std::string const& n)
        : name(n)
    {}

    std::string name;
};

CHARMED_TAG_MEMBER(&my_type::member1, member_name_attribute("member1"));
CHARMED_TAG_MEMBER(&my_type::member2, member_name_attribute("member2"));

BOOST_AUTO_TEST_CASE( metadata_of_test )
{
    BOOST_REQUIRE(query::metadata_of<member_name_attribute>(&my_type::member1));
    BOOST_CHECK(query::metadata_of<member_name_attribute>(&my_type::member1)->name == "member1");

    BOOST_REQUIRE(query::metadata_of<member_name_attribute>(&my_type::member2));
    BOOST_CHECK(query::metadata_of<member_name_attribute>(&my_type::member2)->name == "member2");
}
