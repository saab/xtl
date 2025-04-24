///////////////////////////////////////////////////////////////////////////////
//
//	Description: Contains composite functions for boost variant library
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "objio.h"
#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>
///////////////////////////////////////////////////////////////////////////////
template<class Format, BOOST_VARIANT_ENUM_PARAMS(typename T) >
void composite(obj_output<Format>& output, boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) >& v);
///////////////////////////////////////////////////////////////////////////////
template<class Format>
class xtl_variant_output : public boost::static_visitor<>, boost::noncopyable
{
private:
	obj_output<Format>& output;
public:
	explicit xtl_variant_output(obj_output<Format>& output_) : output(output_) {}

	template <typename T>
	void operator()(const T& operand) const
	{
		output.simple(operand);
	}
};
///////////////////////////////////////////////////////////////////////////////
template<class Format, BOOST_VARIANT_ENUM_PARAMS(typename T) >
void composite(obj_output<Format>& output, boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) >& v)
{
	output.simple(v.which());
	xtl_variant_output<Format> visitor(output);
	v.apply_visitor(visitor);
}
///////////////////////////////////////////////////////////////////////////////

namespace NXTL {
namespace NDetail { // Include internal helpers in a namespace 

template<class Format, class Variant, class T>
inline void xtl_variant_input(obj_input<Format>& input, Variant& v, T*)
{
	v = T{};
	input.simple(boost::get<T>(v));
}

template<class Format, class Variant>
inline void xtl_variant_input(obj_input<Format>& /*input*/, Variant& /*v*/, boost::detail::variant::void_*)
{
	throw std::range_error("XTL invalid variant");
}

/// \brief	Internal helper function.
/// \details	Base case of the recursion, where there are zero types left. But if we
///				reach it, the 'which' was out of bounds (higher than the number of types
///				in the template) so the input was invalid.
template<class Format, typename Variant>
void composite_help(obj_input<Format>& /*input*/, Variant& /*v*/, int /*which*/)
{
	throw std::range_error("XTL invalid variant");
}

/// \brief	Internal helper function.
/// \details	Recursively unwind the template parameter pack
///				to find the one desired based on the index 'which'.
/// \param which	Index of the type of the variant's content.
/// \tparam T		The first type in the variant's template parameter pack.
/// \tparam TN		The tail (0 or more types) of the variant's template parameter pack.
template<class Format, typename Variant, typename T, typename... TN>
void composite_help(obj_input<Format>& input, Variant& v, int which)
{
	if (which == 0) // We want the first type in the pack. Deserialize as a T.
	{
		xtl_variant_input(input, v, (T*)0);
	}
	else // We do not want T, we want one of the types in TN...
	{    // Recursively call this function but with the tail, TN.
		composite_help<Format, Variant, TN...>(input, v, which - 1);
	}
}

}
}

/// \brief Deserializes a boost::variant from an input stream.
/// \param input	The stream to read from.
/// \param v[out]	The result will be stored here.
/// \tparam Format	The format driver of the input stream.
/// \tparam TN		The template parameter pack of the variant type.
///					Example: If 'v' is a boost::variant<int, string>,
///					then TN... expands to 'int, string'.
template<class Format, typename... TN>
void composite(obj_input<Format>& input, boost::variant<TN...>& v)
{
	int which;
	input.simple(which);

	NXTL::NDetail::composite_help<Format, boost::variant<TN...>, TN...>(input, v, which);
}
