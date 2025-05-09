///////////////////////////////////////////////////////////////////////////////
//
//	Description: Contains composite functions for std::shared_ptr
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "objio.h"
#include <memory>
///////////////////////////////////////////////////////////////////////////////
template<class Format, typename T>
inline void composite(obj_input<Format>& stream, std::shared_ptr<T>& p)
{
	T* pt = 0;
	stream.pointer(pt);
	p.reset(pt);
}
///////////////////////////////////////////////////////////////////////////////
template<class Format, typename T>
inline void composite(obj_output<Format>& stream, std::shared_ptr<T>& p)
{
	stream.pointer(p.get());
}
///////////////////////////////////////////////////////////////////////////////
