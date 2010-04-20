///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Index>
    struct erase_single
    {
        typedef typename Index::value_type value_type;
        typedef typename Index::iterator iterator;

        erase_single(Index & i, value_type const& v, iterator & r)
            : index(i)
            , value(v)
            , result(r)
        {}

        template <typename I0>
        void operator()(I0 & x) const
        {
            // if x is the index for which erase was initially called, store the result of erasing, otherwise
            // just erase the element
            if (static_cast<void *>(&x.impl()) == static_cast<void *>(&index.impl()))
            {
                // have to get the element after value this way since boost::intrusive::unordered_set's erase does not
                // return anything
                // TODO: I should NOT need unconst() here...
                result = index.impl().iterator_to(value).unconst();
                ++result;

                x.impl().erase(x.impl().iterator_to(value));
            }
            else
            {
                x.impl().erase(x.impl().iterator_to(value));
            }
        }

        Index & index;
        value_type const& value;
        iterator & result;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_SINGLE_HPP )