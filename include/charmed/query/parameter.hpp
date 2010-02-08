///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_PARAMETER_HPP )
#define CHARMED_QUERY_PARAMETER_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_range.hpp>

#include <charmed/detail/type_index_of.hpp>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/iterator.hpp>

namespace charmed { namespace query
{
    namespace detail
    {
        struct parameter_function_compare
        {
            typedef bool result_type;

            parameter_function_compare(void * f_)
                : f(f_)
            {}

            template <typename T>
            bool operator()(T const& x) const
            {
                typedef std::pair<void *, int> pair_type;
                return static_cast<pair_type const*>(
                    static_cast<metadata<T> const&>(x).type_data_hook)->first == f;
            }

            void * f;
        };
    }

    // parameter_attributes
    namespace result_of
    {
        template <typename M>
        struct parameter_attributes
        {
            typedef boost::iterator_range<
                boost::filter_iterator<
                    detail::parameter_function_compare,
                    typename metadata_iterator<M>::type
                >
            > type;
        };
    }

    template <typename M, typename F>
    inline typename result_of::parameter_attributes<M>::type parameter_attributes(F * f)
    {
        typedef typename result_of::parameter_attributes<M>::type result_type;
        typedef typename boost::range_iterator<result_type>::type iterator;

        typedef typename metadata_storage<M>::container_type container_type;

        container_type const& metadata = metadata_storage<M>::metadata();

        detail::parameter_function_compare pred(f);
        return result_type(iterator(pred, metadata.begin(), metadata.end()), iterator(pred, metadata.end(), metadata.end()));
    }

    // parameter_attribute
    // TODO: If there's an unordered_multiset index that uses F *, both this & the above can be done
    // far faster. This would require the ability to arbitrarily add indices to charmed's indexing mechanism.
    // TODO: There needs to be a way to tie an attribute to a specific use.  This can be done using an attribute<...>
    // wrapper type that uses CRTP. Use would be: attribute<my_attribute_type>.
    template <typename M, typename F>
    inline M const* parameter_attribute(F * f, int n)
    {
        typedef typename metadata_storage<M>::container_type container_type;
        typedef typename container_type::const_iterator iterator;
        typedef std::pair<void *, int> pair_type;

        M const* result = 0;

        container_type const& metadata = metadata_storage<M>::metadata();
        for (iterator i = metadata.begin(); i != metadata.end(); ++i)
        {
            pair_type const* pair = static_cast<pair_type const*>(i->type_data_hook);

            if (pair->first == f && pair->second == n)
            {
                result = &*i;
                break;
            }
        }

        return result;
    }
}}

#endif // #if !defined( CHARMED_QUERY_PARAMETER_HPP )
