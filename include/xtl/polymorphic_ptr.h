///////////////////////////////////////////////////////////////////////////////
//
//	Description: XTL externalization functions for NMisc::polymorphic_ptr
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "objio.h"
#include <Misc/polymorphic_ptr.h>
///////////////////////////////////////////////////////////////////////////////
#define XTL_DUMMY_POINTER(_,N,NAME) (NAME##N*)0
///////////////////////////////////////////////////////////////////////////////
namespace NXTL { namespace NDetail {
	template<class TFormat, typename TBase, typename T>
	void composite_input_ptr(obj_input<TFormat>& stream, 
							TBase*& ptr,
							T* t)
	{
		stream.reference(t);
		ptr = t;
	}

	template<class TFormat, typename TBase>
	void composite_input_ptr(obj_input<TFormat>& /*stream*/, 
							TBase*& /*ptr*/,
							NMisc::NDetail::void_* /*t*/)
	{
		throw std::range_error("XTL invalid choice");
	}

	template<class TFormat, typename TBase, typename T>
	bool composite_output_ptr(obj_output<TFormat>& stream, 
							TBase* ptr,
							int n,
							T* t)
	{
		t = dynamic_cast<T*>(ptr);
		if ( t )
		{
			stream.simple(n);
			stream.reference(t);
			return true;
		}
		return false;
	}

	template<class TFormat, typename TBase>
	bool composite_output_ptr(obj_output<TFormat>& /*stream*/, 
							TBase* /*ptr*/,
							int /*n*/,
							NMisc::NDetail::void_* /*t*/)
	{
		throw std::range_error("XTL invalid choice");
	}

} // NDetail
} // NXTL

template<class TFormat, typename TBase, LILIB_DECLARE_PARAMS>
void composite(obj_input<TFormat>& stream, NMisc::polymorphic_ptr<TBase, LILIB_REF_PARAMS>& ptr)
{
	TBase* temp = 0;
	int discr;
	stream.simple(discr);
	switch( discr )
	{
	case 0:
		break;

#define XTL_POLYMORPHIC_PTR_INPUT_CASE(z, N, _) \
	case BOOST_PP_INC(N): \
		NXTL::NDetail::composite_input_ptr(stream, temp, static_cast<LILIB_REF_PARAM(N)*>(0)); \
		break;
	
	BOOST_PP_REPEAT(BOOST_MPL_LIMIT_LIST_SIZE, XTL_POLYMORPHIC_PTR_INPUT_CASE, _)

#undef XTL_POLYMORPHIC_PTR_INPUT_CASE

	default:
	 	throw std::range_error("XTL invalid choice");
	};
	ptr.reset(temp);
}
template<class TFormat, typename TBase, LILIB_DECLARE_PARAMS>
void composite(obj_output<TFormat>& stream, NMisc::polymorphic_ptr<TBase, LILIB_REF_PARAMS>& ptr)
{
	if ( ptr.get() == 0 )
		stream.simple((int)0);
	else
	{
		bool bOk = false;
#define XTL_POLYMORPHIC_PTR_OUTPUT(z, N, _) \
		if ( !bOk ) \
			bOk = NXTL::NDetail::composite_output_ptr(stream, ptr.get(), BOOST_PP_INC(N), static_cast<LILIB_REF_PARAM(N)*>(0));

		BOOST_PP_REPEAT(BOOST_MPL_LIMIT_LIST_SIZE, XTL_POLYMORPHIC_PTR_OUTPUT, _)

#undef XTL_POLYMORPHIC_PTR_OUTPUT
		if ( !bOk ) 
			throw std::range_error("XTL invalid choice");
	}
}
