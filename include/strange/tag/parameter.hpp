#if !defined( CHARMED_TAG_PARAMETER_HPP )
#define CHARMED_TAG_PARAMETER_HPP

#include <charmed/detail/metadata_initializer.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/typeof/typeof.hpp>

#include <utility>

#define CHARMED_TAG_PARAMETER(f, i, expr)                                                                             \
    std::pair<void *, int>                                                                                            \
        charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::runtime_type_data(f, i);     \
    charmed::detail::metadata_initializer<BOOST_TYPEOF(expr)>                                                         \
        charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::metadata(                    \
            &charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f), i>::type<f>::runtime_type_data, expr)

namespace charmed
{
    // TODO make sure N is within the allowed range for the function's # of arguments (ie, can't tag a parameter of a function
    // that doesn't have any)
    // TODO use preprocessor iteration
    template <typename M, typename F, int N>
    struct parameter_association;

    template <typename M, typename R, int N>
    struct parameter_association<M, R(*)(), N>
    {
        template <R(*F)()>
        struct type
        {
            static std::pair<void *, int> runtime_type_data;
            static detail::metadata_initializer<M> metadata;
        };
    };

    template <typename M, typename R, typename A0, int N>
    struct parameter_association<M, R(*)(A0), N>
    {
        template <R(*F)(A0)>
        struct type
        {
            static std::pair<void *, int> runtime_type_data;
            static detail::metadata_initializer<M> metadata;
        };
    };
}

#endif // #if !defined( CHARMED_TAG_PARAMETER_HPP )
