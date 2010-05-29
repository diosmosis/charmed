///////////////////////////////////////////////////////////////////////////////
//
/// \file variable.hpp
/// Contains the <c>CHARMED_TAG_VARIABLE</c> attribute tagging macro.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_TAG_VARIABLE_HPP )
#define CHARMED_TAG_VARIABLE_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/metadata_initializer.hpp>
#include <boost/typeof/typeof.hpp>

/// \brief Associates the attribute that results from <c>expr</c> with the
///        global variable <c>var</c>.
///
/// \remark <c>var</c> must exist for the entire lifetime of the application, otherwise
///         the result of the <c>tagged_type_of()</c> free function will be invalid for
///         <c>var</c>'s associated attribute.
///
/// \param var the data to associate an attribute with.
/// \param n a positive integer that is unique in every use of this macro in the translation
///        unit where this macro is used.
/// \param expr an expression that results in a constructed attribute.
#define CHARMED_TAG_VARIABLE(var, n, expr)                                                                            \
    static charmed::metadata_initializer<BOOST_TYPEOF(expr)> _charmed_mi_ ## n(&var, expr);

#endif // #if !defined( CHARMED_TAG_VARIABLE_HPP )
