///////////////////////////////////////////////////////////////////////////////
//
//	Description: Contains composite functions for the STL hash_map class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "objio.h"
#include <queue>
///////////////////////////////////////////////////////////////////////////////
template <class Stream, class T, class A>
inline void composite(Stream& stream, std::queue<T, A>& data) {
	stream.container(data);
}
///////////////////////////////////////////////////////////////////////////////
template <class Stream, class T, class C, class Pr>
inline void composite(Stream& stream, std::priority_queue<T, C, Pr>& data) {
	stream.container(NMisc::GetContainer(data));
}
///////////////////////////////////////////////////////////////////////////////
