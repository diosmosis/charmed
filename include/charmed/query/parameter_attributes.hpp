///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_PARAMETER_ATTRIBUTES_HPP )
#define CHARMED_QUERY_PARAMETER_ATTRIBUTES_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_range.hpp>

#include <charmed/detail/type_index_of.hpp>

#include <boost/iterator/filter_iterator.hpp>

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
            bool operator()(T & x) const
            {
                typedef std::pair<void *, int> pair_type;
                return static_cast<pair_type const*>(x.type_data_hook)->first == f;
            }

            void * f;
        };
    }

    template <typename M, typename F>
    inline boost::iterator_range<
        boost::filter_iterator<
            detail::parameter_function_compare,
            typename metadata_iterator<M>::type
        >
    > parameter_attributes(F * f)
    {
        typedef boost::filter_iterator<
            detail::parameter_function_compare,
            typename metadata_iterator<M>::type
        > iterator;
        typedef boost::iterator_range<iterator> result_type;
        typedef charmed::detail::type_index_of<M> idx;
        typedef typename metadata_storage<M>::container_type container_type;

        container_type const& metadata = metadata_storage<M>::metadata();

        detail::parameter_function_compare pred(f);
        return result_type(iterator(pred, metadata.begin(), metadata.end()), iterator(pred, metadata.end(), metadata.end()));
    }
}}

#endif // #if !defined( CHARMED_QUERY_PARAMETER_ATTRIBUTES_HPP )
