///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
template <typename T0, typename T1 = int, typename T2 = int> struct lazy_false : boost::mpl::false_ {};
// TODO: review & check if more test cases should be added (such as for projection)
#define BOOST_TEST_MODULE intrusive_multi_index
#include <boost/test/included/unit_test.hpp>
#include <boost/multi_index/intrusive/multi_index_container.hpp>
#include <boost/multi_index/intrusive/ordered_index.hpp>
#include <boost/multi_index/intrusive/unordered_index.hpp>
#include <boost/multi_index/intrusive/sequenced_index.hpp>
#include <boost/multi_index/intrusive/composite_hook.hpp>
#include <boost/multi_index/intrusive/fusion_hook.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <list>

using namespace boost::multi_index;

struct value_type_impl
{
    value_type_impl(int ouk, int onuk, int uuk, int unuk)
        : ordered_unique_key(ouk)
        , ordered_non_unique_key(onuk)
        , unordered_unique_key(uuk)
        , unordered_non_unique_key(unuk)
    {}

    value_type_impl(value_type_impl const& x)
        : ordered_unique_key(x.ordered_unique_key)
        , ordered_non_unique_key(x.ordered_non_unique_key)
        , unordered_unique_key(x.unordered_unique_key)
        , unordered_non_unique_key(x.unordered_non_unique_key)
    {}

    int ordered_unique_key;
    int ordered_non_unique_key;
    int unordered_unique_key;
    int unordered_non_unique_key;
};

// TODO: composite_hook was written specifically for charmed. it should be possible to use the intrusive multi_index
// without composite_hook. In other words, it should be possible to divvy up the hooks and specify each one in
// the multi_index_container template, instead of having the value_type_impl class above.
struct value_type : value_type_impl
{
    typedef indexed_by<
        intrusive::sequenced,
        intrusive::ordered_unique<member<value_type_impl, int, &value_type_impl::ordered_unique_key> >,
        intrusive::ordered_non_unique<member<value_type_impl, int, &value_type_impl::ordered_non_unique_key> >,
        intrusive::unordered_unique<member<value_type_impl, int, &value_type_impl::unordered_unique_key> >,
        intrusive::unordered_non_unique<member<value_type_impl, int, &value_type_impl::unordered_non_unique_key> >
    > index_specifier;

    typedef intrusive::composite_hook<index_specifier> hook_type;

    value_type(int ouk, int onuk, int uuk, int unuk)
        : value_type_impl(ouk, onuk, uuk, unuk)
    {}

    value_type(value_type const& x)
        : value_type_impl(x)
        , hook()
    {}

    hook_type hook;
};

typedef intrusive::multi_index_container<
    value_type,
    value_type::index_specifier,
    intrusive::fusion_hook_specifier<value_type, value_type::hook_type, &value_type::hook>
> container;

// helper functions
struct is_same_object
{
    is_same_object(value_type & v)
        : value(v)
    {}

    bool operator()(value_type & v) const
    {
        return &v == &value;
    }

    value_type & value;
};

static void fill_container(container & c, std::list<value_type> & values)
{
    values.push_back(value_type(0, 0, 0, 0));
    c.push_back(values.back());

    values.push_back(value_type(1, 1, 1, 1));
    c.push_back(values.back());

    values.push_back(value_type(2, 2, 2, 2));
    c.push_back(values.back());

    values.push_back(value_type(3, 3, 3, 3));
    c.push_back(values.back());
}

static void check_indices_contain(container & c, value_type & value)
{
    BOOST_CHECK(std::find_if(c.begin(), c.end(), is_same_object(value)) != c.end());
    BOOST_CHECK(std::find_if(c.get<1>().begin(), c.get<1>().end(), is_same_object(value)) != c.get<1>().end());
    BOOST_CHECK(std::find_if(c.get<2>().begin(), c.get<2>().end(), is_same_object(value)) != c.get<2>().end());
    BOOST_CHECK(std::find_if(c.get<3>().begin(), c.get<3>().end(), is_same_object(value)) != c.get<3>().end());
    BOOST_CHECK(std::find_if(c.get<4>().begin(), c.get<4>().end(), is_same_object(value)) != c.get<4>().end());
}

static void check_indices_dont_contain(container & c, value_type & value)
{
    BOOST_CHECK(std::find_if(c.begin(), c.end(), is_same_object(value)) == c.end());
    BOOST_CHECK(std::find_if(c.get<1>().begin(), c.get<1>().end(), is_same_object(value)) == c.get<1>().end());
    BOOST_CHECK(std::find_if(c.get<2>().begin(), c.get<2>().end(), is_same_object(value)) == c.get<2>().end());
    BOOST_CHECK(std::find_if(c.get<3>().begin(), c.get<3>().end(), is_same_object(value)) == c.get<3>().end());
    BOOST_CHECK(std::find_if(c.get<4>().begin(), c.get<4>().end(), is_same_object(value)) == c.get<4>().end());
}

static void check_indices_empty(container & c)
{
    BOOST_CHECK(c.size() == 0);
    BOOST_CHECK(c.begin() == c.end());

    BOOST_CHECK(c.get<1>().size() == 0);
    BOOST_CHECK(c.get<1>().begin() == c.get<1>().end());

    BOOST_CHECK(c.get<2>().size() == 0);
    BOOST_CHECK(c.get<2>().begin() == c.get<2>().end());

    BOOST_CHECK(c.get<3>().size() == 0);
    BOOST_CHECK(c.get<3>().begin() == c.get<3>().end());

    BOOST_CHECK(c.get<4>().size() == 0);
    BOOST_CHECK(c.get<4>().begin() == c.get<4>().end());
}

// test cases
BOOST_AUTO_TEST_CASE( container_construct_accuracy )
{
    container c;
}

BOOST_AUTO_TEST_CASE( sequenced_insert_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value(4, 4, 4, 4);
        std::pair<container::iterator, bool> i = c.insert(--c.end(), value);

        // check that the item was inserted
        BOOST_CHECK(i.second && &*(i.first) == &value);

        // check that the item was inserted in the right place
        BOOST_CHECK(std::distance(c.begin(), i.first) == 3);

        // check that the item is in every other index
        check_indices_contain(c, value);
    }
}

BOOST_AUTO_TEST_CASE( sequenced_push_back_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        BOOST_CHECK(c.size() == 4);

        container::iterator i = c.begin();
        std::list<value_type>::iterator vi = all_values.begin();

        // check that each value in all_values was push_backed to c
        for (std::size_t j = 0; j != all_values.size(); ++j, ++i, ++vi)
        {
            BOOST_CHECK(&*i == &*vi);
            check_indices_contain(c, *vi);
        }
    }
}

BOOST_AUTO_TEST_CASE( sequenced_pop_back_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.pop_back();

        // check that the value is absent from every index
        check_indices_dont_contain(c, all_values.back());
    }
}

BOOST_AUTO_TEST_CASE( sequenced_push_front_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value(4, 4, 4, 4);
        c.push_front(value);

        // check that the item was added & in the right place
        BOOST_CHECK(&c.front() == &value);

        // check that the item is in every other index
        check_indices_contain(c, value);
    }
}

BOOST_AUTO_TEST_CASE( sequenced_pop_front_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.pop_front();

        // check that the value is absent from every index
        check_indices_dont_contain(c, all_values.front());
    }
}

BOOST_AUTO_TEST_CASE( sequenced_erase_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        container::iterator i = c.begin();
        ++i;
        c.erase(i);

        std::list<value_type>::iterator vi = ++all_values.begin();

        // check that the item was removed from every index
        check_indices_dont_contain(c, *vi);
    }
}

BOOST_AUTO_TEST_CASE( sequenced_clear_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.clear();

        check_indices_empty(c);
    }
}

BOOST_AUTO_TEST_CASE( sequenced_insert_unique_failure )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value(0, 4, 4, 4);

        BOOST_CHECK(!c.push_back(value).second);
        BOOST_CHECK(!c.push_front(value).second);
        BOOST_CHECK(!c.insert(--c.end(), value).second);
    }
}

BOOST_AUTO_TEST_CASE( sequenced_insert_nonunique_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value1(4, 1, 4, 2);
        value_type value2(5, 1, 5, 2);
        value_type value3(6, 1, 6, 2);

        c.push_back(value1);
        check_indices_contain(c, value1);

        c.push_front(value2);
        check_indices_contain(c, value2);

        c.insert(--c.end(), value3);
        check_indices_contain(c, value3);
    }
}

BOOST_AUTO_TEST_CASE( associative_erase_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.get<1>().erase(all_values.front());

        check_indices_dont_contain(c, all_values.front());
    }
}

BOOST_AUTO_TEST_CASE( associative_clear_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.get<1>().clear();

        check_indices_empty(c);
    }
}

BOOST_AUTO_TEST_CASE( associative_insert_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value(4, 4, 4, 4);
        c.get<1>().insert(value);

        // check that the back of the sequenced index contains value
        BOOST_CHECK(&c.back() == &value);

        // check all indices contain the value
        check_indices_contain(c, value);
    }
}

BOOST_AUTO_TEST_CASE( associative_insert_unique_failure )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value(0, 4, 4, 4);

        BOOST_CHECK(!c.get<2>().insert(value).second);

        check_indices_dont_contain(c, value);
    }
}

BOOST_AUTO_TEST_CASE( associative_insert_nonunique_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        value_type value1(4, 1, 4, 2);
        value_type value2(5, 1, 5, 2);

        c.get<2>().insert(value1);
        check_indices_contain(c, value1);

        c.get<4>().insert(value2);
        check_indices_contain(c, value2);
    }
}

BOOST_AUTO_TEST_CASE( modify_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        // TODO
    }
}

BOOST_AUTO_TEST_CASE( modify_failure )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        // TODO
    }
}

BOOST_AUTO_TEST_CASE( replace_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        // TODO
    }
}

BOOST_AUTO_TEST_CASE( replace_failure )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        // TODO
    }
}

BOOST_AUTO_TEST_CASE( erase_multiple_accuracy )
{
    std::list<value_type> all_values;
    {
        container c;

        fill_container(c, all_values);

        c.erase(++c.begin(), c.end());

        std::list<value_type>::iterator i = all_values.begin();
        check_indices_contain(c, *i);
        check_indices_dont_contain(c, *++i);
        check_indices_dont_contain(c, *++i);
        check_indices_dont_contain(c, *++i);
    }
}
