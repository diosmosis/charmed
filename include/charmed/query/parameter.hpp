///////////////////////////////////////////////////////////////////////////////
//
/// \parameter.hpp
/// Contains functions that query the attributes associated with function parameters,
/// including <c>parameter_attributes()</c> and <c>parameter_attribute</c>.
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
#include <charmed/query/metadata_of.hpp>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/function_types/function_arity.hpp>

namespace charmed { namespace query
{
    namespace result_of
    {
        /// \brief A metafunction that returns the result type of the <c>parameter_attributes\<M\>()</c>
        ///        free function.
        ///
        /// <c>parameter_attributes\<\></c> will return an iterator range iterating <c>M const</c>s.
        ///
        /// \tparam M the attribute type.
        template <typename M>
        struct parameter_attributes
        {
            struct filter_null_attribute
            {
                template <typename M>
                bool operator()(M const* m) const
                {
                    return m != 0;
                }
            };

            struct iterator
                : boost::iterator_adaptor<
                    iterator,
                    boost::filter_iterator<
                        filter_null_attribute,
                        metadata<M> const* const*
                    >,
                    M const
                >
            {
                typedef typename iterator::iterator_adaptor_ base_type;
                typedef metadata<M> const* const* pointer_type;

                iterator()
                    : base_type()
                {}

                iterator(pointer_type i)
                    : base_type(i)
                {}

                iterator(iterator const& x)
                    : base_type(x)
                {}

                iterator & operator = (iterator const& x)
                {
                    base_type::operator = (x);
                    return *this;
                }

            private:
                friend class boost::iterator_core_access;

                M const& dereference() const
                {
                    return **base();
                }
            };

            typedef boost::iterator_range<iterator> type;
        };
    }

    /// \brief Gets the range of attributes of type <c>M</c> associated with the parameters
    ///        of <c>f</c>.
    ///
    /// \tparam M the attribute type.
    /// \param f the function whose parameters' attributes are to be retrieved.
    /// \result all the attributes associated with the parameters and return type of <c>f</c>.
    template <typename M, typename F>
    inline typename result_of::parameter_attributes<M>::type parameter_attributes(F * f)
    {
        typedef typename result_of::parameter_attributes<M>::type result_type;
        typedef param_range_attribute<M, boost::function_types::function_arity<F>::value> pr_attr;

        pr_attr const* all = metadata_of<pr_attr>(f);

        if (!all)
        {
            return result_type();
        }

        return result_type(all->begin(), all->end());
    }

    // TODO: There needs to be a way to tie an attribute to a specific use.  This can be done using an attribute<...>
    // wrapper type that uses CRTP. Use would be: function_attribute<my_attribute_type>.

    /// \brief Gets the attribute, if any, associated with the <c>n</c>th parameter of
    ///        <c>f</c>.
    ///
    /// \tparam M the attribute type.
    /// \param f the function whose parameter's attribute is being retrieved.
    /// \param n the index of the parameter to get. Passing <c>-1</c> will access the
    ///          return type's attribute (if any).
    /// \result the attribute associated with <c>f</c>'s <c>n</c>th parameter. If there
    ///         is no associated attribute, <c>0</c> is returned.
    template <typename M, typename F>
    inline M const* parameter_attribute(F * f, int n)
    {
        typedef param_range_attribute<M, boost::function_types::function_arity<F>::value> pr_attr;

#if defined( CHARMED_NOTHROW )
        BOOST_ASSERT(n >= -1 && n < pr_attr::N);
#else
        if (n < -1 || n >= pr_attr::N)
        {
            throw std::runtime_error("in 'charmed::parameter_attribute': n does not reference a valid parameter of f");
        }
#endif

        pr_attr const* all = metadata_of<pr_attr>(f);

        if (!all)
        {
            return 0;
        }

        return all->pdata[n + 1];
    }
}}

#endif // #if !defined( CHARMED_QUERY_PARAMETER_HPP )
