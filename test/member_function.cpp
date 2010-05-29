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
#include <charmed/tag/member_function.hpp>
#include <charmed/query/metadata_of.hpp>
#include <charmed/query/tagged_type_of.hpp>
#include <string>

using namespace charmed;

struct my_type
{
    virtual ~my_type() {}

    void do_something() const
    {
    }

    int operator()(int v)
    {
        return v;
    }

    virtual void virtual_op() const = 0;
};

struct my_derived_type
    : my_type
{
    void virtual_op() const
    {
    }
};

struct mem_fun_name_attribute
{
    mem_fun_name_attribute(std::string const& n)
        : name(n)
    {}

    std::string name;
};

CHARMED_TAG_MEMBER_FUNCTION(&my_type::do_something, mem_fun_name_attribute("my_type::do_something"));
CHARMED_TAG_MEMBER_FUNCTION(&my_type::operator(), mem_fun_name_attribute("my_type::operator()"));
CHARMED_TAG_MEMBER_FUNCTION(&my_type::virtual_op, mem_fun_name_attribute("my_type::virtual_op"));
CHARMED_TAG_MEMBER_FUNCTION(&my_derived_type::virtual_op, mem_fun_name_attribute("my_derived_type::virtual_op"));

BOOST_AUTO_TEST_CASE( metadata_of_test )
{
    BOOST_REQUIRE(query::metadata_of<mem_fun_name_attribute>(&my_type::do_something));
    BOOST_CHECK(query::metadata_of<mem_fun_name_attribute>(&my_type::do_something)->name == "my_type::do_something");

    BOOST_REQUIRE(query::metadata_of<mem_fun_name_attribute>(&my_derived_type::do_something));
    BOOST_CHECK(query::metadata_of<mem_fun_name_attribute>(&my_derived_type::do_something)->name == "my_type::do_something");

    BOOST_REQUIRE(query::metadata_of<mem_fun_name_attribute>(&my_type::operator()));
    BOOST_CHECK(query::metadata_of<mem_fun_name_attribute>(&my_type::operator())->name == "my_type::operator()");

    BOOST_REQUIRE(query::metadata_of<mem_fun_name_attribute>(&my_type::virtual_op));
    BOOST_CHECK(query::metadata_of<mem_fun_name_attribute>(&my_type::virtual_op)->name == "my_type::virtual_op");

    BOOST_REQUIRE(query::metadata_of<mem_fun_name_attribute>(&my_derived_type::virtual_op));
    BOOST_CHECK(query::metadata_of<mem_fun_name_attribute>(&my_derived_type::virtual_op)->name == "my_derived_type::virtual_op");
}
