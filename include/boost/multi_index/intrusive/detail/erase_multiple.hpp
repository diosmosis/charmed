#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_MULTIPLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_MULTIPLE_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Index>
    struct erase_multiple
    {
        typedef typename Index::iterator iterator;

        erase_multiple(Index & i, iterator f, iterator l)
            : index(i)
            , first(f)
            , last(l)
        {}

        template <typename I0>
        void operator()(I0 & x) const
        {
            if (static_cast<void *>(&x.impl()) != static_cast<void *>(&index.impl()))
            {
                x.impl().erase(x.impl().iterator_to(*first), x.impl().iterator_to(*last));
            }
        }

        Index & index;
        iterator first
               , last;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_MULTIPLE_HPP )