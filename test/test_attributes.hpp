///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_TEST_TEST_ATTRIBUTES_HPP )
#define CHARMED_TEST_TEST_ATTRIBUTES_HPP

#include <charmed/tag/function.hpp>
#include <charmed/tag/type.hpp>
#include <charmed/tag/member.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/intrusive/sequenced_index.hpp>
#include <boost/multi_index/intrusive/ordered_index.hpp>
#include <string>

struct my_type
{
    int my_var;
};

typedef void (my_function_type)(int, int);

void my_function(int a, char b)
{
    // empty
}

struct my_attribute
{
    my_attribute(std::string const& k, int ik, int c)
        : key(k)
        , int_key(ik)
        , count(c)
    {}

    std::string key;
    int int_key;
    int count;
};

namespace charmed
{
    template <>
    struct index_specifier_of<my_attribute>
    {
        typedef boost::multi_index::indexed_by<
            boost::multi_index::intrusive::sequenced,
            boost::multi_index::intrusive::ordered_unique<
                boost::multi_index::member<my_attribute, std::string, &my_attribute::key>
            >,
            boost::multi_index::intrusive::ordered_non_unique<
                boost::multi_index::member<my_attribute, int, &my_attribute::int_key>
            >
        > type;
    };
}

CHARMED_TAG_TYPE(my_type, my_attribute("a", 2, 0));
CHARMED_TAG_FUNCTION(&my_function, my_attribute("c", 2, 1));
CHARMED_TAG_MEMBER(&my_type::my_var, my_attribute("d", 2, 2));
CHARMED_TAG_TYPE(my_function_type, my_attribute("b", 4, 3));

#endif // #if !defined( CHARMED_TEST_TEST_ATTRIBUTES_HPP )
