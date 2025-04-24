#pragma warning(disable:4786)  // 'identifier' : identifier was truncated to 'number' characters in the debug information
#pragma warning(push)
#pragma warning(disable:4512)  // assignment operator could not be generated
/*
 * Core externaliztion utilities
 *
 * Copyright (C) 1998-2000 Jose' Orlando Pereira, jop@di.uminho.pt
 */
/* XTL - eXternalization Template Library - http://gsd.di.uminho.pt/~jop/xtl
 * Copyright (C) 1998-2000 Jose' Orlando Pereira, Universidade do Minho
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA
 *
 * $Id: objio.h,v 1.4 2000/12/22 12:19:50 jop Exp $
 */

#ifndef __XTL_OBJIO
#define __XTL_OBJIO

#include "config.h"

#include <string>

#define XTL_COMPOSE_ENUM(E) template <class S> void composite (S& s, E& x){s.simple((int&) x);}

template <class Buffer>
class generic_format {
private:
	Buffer& buffer;
protected:
	void* require(std::size_t n)	{return buffer.require(n);}
	void unrequire(std::size_t n)	{buffer.unrequire(n);}
	void* desire(std::size_t n)	{return buffer.desire(n);}
	void undesire(std::size_t n)	{buffer.undesire(n);}

	void read(char* data, std::size_t size)		{buffer.read(data, size);}
	void write(char const* data, std::size_t size)	{buffer.write(data, size);}
public:
 	generic_format(Buffer& buf):buffer(buf) {}

	void input_start() {}

	void input_start_composite() {}
	void input_end_composite() {}

	void input_start_vector() {}
	void input_end_vector() {}

	virtual void output_start() {}

	void output_start_composite() {}
	void output_end_composite() {}

	void output_start_vector() {}
	void output_end_vector() {}

	void reset_buffer() {
		buffer.rewind();
		output_start();
	}

	Buffer& GetBuffer() { return buffer; }
};

template <class Buffer>
class raw_format: public generic_format<Buffer> {
 public:
 	raw_format(Buffer& buf):generic_format<Buffer>(buf) {}
	
	template <class T>
	inline void input_simple(T& data)
		{data=*((T*)this->require(sizeof(T)));}

	template <class Idx>
	void input_start_array(Idx& n) {input_simple(n);}
	template <class Idx>
		bool input_end_array(Idx& n) { return (n-- == 0); }

	void input_chars(char* data, std::size_t size) {input_raw(data, size);}

	void input_raw(char* data, std::size_t size) {
		std::size_t i;
		for(i=0;i<(size>>8);i++,data+=256)
			memcpy(data, this->require(256), 256);
		std::size_t res=size-(i<<8);
		memcpy(data, this->require(res), res);
	}

	template <class T>
	inline void output_simple(T const& data)
		{*((T*)this->desire(sizeof(T)))=data;}

	template <class Idx>
	void output_start_array(Idx n) {output_simple(n);}
	void output_end_array() {}

	void output_chars(char const* data, std::size_t size) {output_raw(data, size);}

	void output_raw(char const* data, std::size_t size) {
		std::size_t i;
		for(i=0;i<(size>>8);i++,data+=256)
			memcpy(this->desire(256), data, 256);
		std::size_t res=size-(i<<8);
		memcpy(this->desire(res), data, res);
	}
};

#ifdef XTL_CONFIG_CHOICE_MACROS
#include <xtl/macros.h>
#endif

class no_refs;

#define def_simple_input(type) \
	inline obj_input& simple(type& data) { \
		format.input_simple(data); \
		return *this; \
	}

template <class Format, class References=no_refs>
class obj_input {
 private:
	Format format;
 	References refs;

 public:
	template <class Buffer>
		obj_input(Buffer& buf) : format(buf)
	{ format.input_start(); }

	Format& GetFormat() { return format; }

#ifdef XTL_CONFIG_COMPOSITE_BUG
	// Moved here by frsj1
	template <class T>
	inline obj_input& simple(T& data) {
		format.input_start_composite();
		::composite(*this, data);
		format.input_end_composite();
		return *this;
	}
#endif

	def_simple_input(bool);
	def_simple_input(char);
	def_simple_input(unsigned char);
	def_simple_input(short);
	def_simple_input(unsigned short);
	def_simple_input(XTL_CONFIG_W64 int);
#ifdef XTL_CONFIG_EXTRAINTTYPES
	def_simple_input(__int8);
	def_simple_input(__int16);
	def_simple_input(__int32);
	def_simple_input(__int64);
#endif
	def_simple_input(XTL_CONFIG_W64 unsigned int);
	def_simple_input(long);
	def_simple_input(unsigned long);
	def_simple_input(longlong);
	def_simple_input(unsignedlonglong);
	def_simple_input(float);
	def_simple_input(double);

	inline obj_input& cstring(char*& data) {
		std::size_t size;
		format.input_start_array(size);
		data=new char[size+1];
		format.input_chars(data, size);
		data[size]=0;
		format.input_end_array(size);
		return *this;
	}

	inline obj_input& cstring(char* data, std::size_t max) {
		std::size_t size;
		format.input_start_array(size);
		format.input_chars(data, size>max?max:size);
		data[size>max?max:size]=0;
		format.input_end_array(size);
		return *this;
	}

	inline obj_input& simple(std::string& data) {
		std::size_t size;
		format.input_start_array(size);
		if ( size != 0 )
		{
			data.resize(size);
			format.input_chars(const_cast<char*>(data.data()), size);
		}
		else
			data = std::string();
		format.input_end_array(size);
		return *this;
	}

	template <class T>
	inline obj_input& vector(T data[], std::size_t size) {
		format.input_start_vector();
		for(std::size_t i=0;i!=size;i++)
			simple(data[i]);
		format.input_end_vector();
		return *this;
	}

	template <class T, class Idx>
	inline obj_input& array(T*& data, Idx& size) {
		format.input_start_array(size);
		data=new T[size];
		for(std::size_t j=size,i=0;!format.input_end_array(j);i++)
			simple(data[i]);
		return *this;
	}

//#ifdef XTL_CONFIG_COMPOSITE_BUG
//	template <class T>
//	inline obj_input& vector_s(T data[], std::size_t size) {
//		format.input_start_vector();
//		for(std::size_t i=0;i<size;i++)
//			simple(data[i]);
//		format.input_end_vector();
//		return *this;
//	}
//
//	template <class T, class Idx>
//	inline obj_input& array_s(T*& data, Idx& size) {
//		format.input_start_array(size);
//		data=new T[size];
//		for(std::size_t j=size,i=0;!format.input_end_array(j);i++)
//			simple(data[i]);
//		return *this;
//	}
//#endif

	// Note: type of 'data' changed from char[] to char* to enable support for
	// data buffers that might not be stored as char[] (It's impossible to convert
	// from char* to char[] but not the other way around) /steyjo
	inline obj_input& opaque(char* data, std::size_t size) {
		format.input_start_vector();
		format.input_raw(data, size);
		format.input_end_vector();
		return *this;
	}

	template <class Idx>
	inline obj_input& bytes(char*& data, Idx& size) {
		std::size_t s=size;
		format.input_start_array(size);
		data=new char[size];
		format.input_raw(data, size);
		format.input_end_array(s);
		return *this;
	}

	template <class Idx>
	inline obj_input& bytes(unsigned __int8*& data, Idx& size) {
		return bytes(reinterpret_cast<char*&>(data), size);
	}

	typedef obj_input<Format, References> thistype;

	// This is for compilers which do not support specialization
	// If possible, use simple(T&) below.
	template <class T>
	inline obj_input& composite(T& data) {
		format.input_start_composite();
		::composite(*this, data);
		format.input_end_composite();
		return *this;
	}

#ifndef XTL_CONFIG_COMPOSITE_BUG
	template <class T>
	inline obj_input& simple(T& data) {
		format.input_start_composite();
		::composite(*this, data);
		format.input_end_composite();
		return *this;
	}
#endif

	template <class T>
	inline obj_input& reference(T*& data) {
		refs.reference(*this, data);
		return *this;
	}

	template <class T>
	inline obj_input& pointer(T*& data) {
		bool exists;
		simple(exists);
		if (exists) 
			reference(data);
		return *this;
	}

	template <class T>
	inline obj_input& container(T& data) {
		std::size_t j=0;
		data.clear();
		format.input_start_array(j);
		while(!format.input_end_array(j)) {
			typename T::value_type v;
			simple(v);
			data.insert(data.end(), v);
		}
		return *this;
	}

	template <class T>
		inline obj_input& map(T& data) {
		std::size_t j=0;
		data.clear();
		format.input_start_array(j);
		while(!format.input_end_array(j)) {
			std::pair<T::key_type, T::mapped_type> v;
			simple(v);
			data.insert(data.end(), v);
		}
		return *this;
	}

	template<class FwdIt>
	inline obj_input& fwd_iterator(FwdIt& start, FwdIt& iter)
	{
		std::size_t n;
		simple(n);
		for ( FwdIt iter = start; n != 0; ++ iter, --n )
			;
		return *this;
	}

	template<class RanIt>
	inline obj_input& ran_iterator(RanIt& start, RanIt& iter)
	{
		std::size_t n;
		simple(n);
		iter = start + n;
		return *this;
	}

	template <class Idx>
	inline obj_input& buffer(char* data, Idx size) {
		return *this;
	}

	template <class Idx>
	inline obj_input& padding(char padding, Idx size) {
		for (std::size_t i=0;i!=size;++i)
			simple(padding);
		return *this;
	}

#ifdef XTL_CONFIG_CHOICE_MACROS
	decl_ich_method(2)
	decl_ich_method(3)
	decl_ich_method(4)
	decl_ich_method(5)
	decl_ich_method(6)
	decl_ich_method(7)
	decl_ich_method(8)
	decl_ich_method(9)
	decl_ich_method(10)
	decl_ich_method(11)
	decl_ich_method(12)
	decl_ich_method(13)
	decl_ich_method(14)
	decl_ich_method(15)

	decl_iobj_method(1)
	decl_iobj_method(2)
	decl_iobj_method(3)
	decl_iobj_method(4)
	decl_iobj_method(5)
	decl_iobj_method(6)
	decl_iobj_method(7)
	decl_iobj_method(8)
	decl_iobj_method(9)
	decl_iobj_method(10)
	decl_iobj_method(11)
	decl_iobj_method(12)
	decl_iobj_method(13)
	decl_iobj_method(14)
	decl_iobj_method(15)
#endif
};

#define def_simple_output(type) \
	inline obj_output& simple(type const& data) { \
		format.output_simple(data); \
		return *this; \
	}

template <class Format, class References=no_refs>
class obj_output {
 private:
 	Format format;
 	References refs;

 public:
	template<class Buffer>
		obj_output(Buffer& buf) : format(buf)
	{ format.output_start(); }

	Format& GetFormat() { return format; }

#ifdef XTL_CONFIG_COMPOSITE_BUG
	// Moved here by frsj1
	template <class T>
	inline obj_output& simple(T const& data) {
		format.output_start_composite();
		::composite(*this, const_cast<T&>(data));
		format.output_end_composite();
		return *this;
	}
#endif

	def_simple_output(bool);
	def_simple_output(char);
	def_simple_output(unsigned char);
	def_simple_output(short);
	def_simple_output(unsigned short);
	def_simple_output(XTL_CONFIG_W64 int);
#ifdef XTL_CONFIG_EXTRAINTTYPES
	def_simple_output(__int8);
	def_simple_output(__int16);
	def_simple_output(__int32);
	def_simple_output(__int64);
#endif
	def_simple_output(XTL_CONFIG_W64 unsigned int);
	def_simple_output(long);
	def_simple_output(unsigned long);
	def_simple_output(longlong);
	def_simple_output(unsignedlonglong);
	def_simple_output(float);
	def_simple_output(double);

	inline obj_output& cstring(char const* data) {
		std::size_t size=strlen(data);
		format.output_start_array(size);
		format.output_chars(data, size);
		format.output_end_array();
		return *this;
	}

	inline obj_output& cstring(char const* data, std::size_t max) {
		std::size_t size=strlen(data);
		format.output_start_array(size);
		format.output_chars(data, size);
		format.output_end_array();
		return *this;
	}

	inline obj_output& simple(const std::string& data) {
		std::size_t size=data.size();
		format.output_start_array(size);
		format.output_chars(data.data(), size);
		format.output_end_array();
		return *this;
	}

	template <class T>
	inline obj_output& vector(const T data[], std::size_t size) {
		format.output_start_vector();
		for(std::size_t i=0;i!=size;i++)
			simple(data[i]);
		format.output_end_vector();
		return *this;
	}

	template <class T, class Idx>
	inline obj_output& array(T const* data, Idx size) {
		format.output_start_array(size);
		for(std::size_t i=0;i!=size;i++)
			simple(data[i]);
		format.output_end_array();
		return *this;
	}
//#ifdef XTL_CONFIG_COMPOSITE_BUG
//	template <class T>
//	inline obj_output& vector_s(const T data[], std::size_t size) {
//		format.output_start_vector();
//		for(std::size_t i=0;i<size;i++)
//			simple(data[i]);
//		format.output_end_vector();
//		return *this;
//	}
//
//	template <class T, class Idx>
//	inline obj_output& array_s(T const* data, Idx size) {
//		format.output_start_array(size);
//		for(std::size_t i=0;i<size;i++)
//			simple(data[i]);
//		format.output_end_array();
//		return *this;
//	}
//#endif

	inline obj_output& opaque(const char data[], std::size_t size) {
		format.output_start_vector();
		format.output_raw(data, size);
		format.output_end_vector();
		return *this;
	}

	template <class Idx>
	inline obj_output& bytes(char const* data, Idx size) {
		format.output_start_array(size);
		format.output_raw(data, size);
		format.output_end_array();
		return *this;
	}

	template <class Idx>
	inline obj_output& bytes(unsigned __int8 const* data, Idx size) {
		return bytes(reinterpret_cast<char const*>(data), size);
	}

	// This is for compilers which do not support specialization
	// If possible, use simple(T&) below.
	template <class T>
	inline obj_output& composite(T const& data) {
		format.output_start_composite();
		::composite(*this, const_cast<T&>(data));
		format.output_end_composite();
		return *this;
	}

#ifndef XTL_CONFIG_COMPOSITE_BUG
	template <class T>
	inline obj_output& simple(T const& data) {
		format.output_start_composite();
		::composite(*this, const_cast<T&>(data));
		format.output_end_composite();
		return *this;
	}
#endif

	template <class T>
	inline obj_output& reference(T const* data) {
		refs.reference(*this, data);
		return *this;
	}

	template <class T>
	inline obj_output& pointer(T const* data) {
		bool exists=data!=NULL;
		simple(exists);
		if (exists)
			reference(data);
		return *this;
	}

	template <class T>
	inline obj_output& container(T const& data) {
		std::size_t j= data.size();
		format.output_start_array(j);
		for(typename T::const_iterator i=data.begin();
			i!=data.end();
			i++)
			simple(*i);
		format.output_end_array();
		return *this;
	}

	template<class T>
	inline obj_output& map(T const& data) {
		return container(data);
	}

	template<class FwdIt>
	inline obj_output& fwd_iterator(FwdIt& start, FwdIt& iter)
	{
		std::size_t n = 0;
		for ( FwdIt t = start; t != iter; t ++, n ++ )
			;
		simple(n);
		return *this;
	}


	template<class RanIt>
	inline obj_output& ran_iterator(RanIt& start, RanIt& iter)
	{
		std::size_t n = iter - start;
		simple(n);
		return *this;
	}

	template<class Idx>
	inline obj_output& buffer(char const* data, Idx size) {
		format.output_raw(data, size);
		return *this;
	}

	template <class Idx>
	inline obj_output& padding(char padding, Idx size) {
		for (std::size_t i=0;i!=(std::size_t)size;++i)
			simple(padding);
		return *this;
	}

#ifdef XTL_CONFIG_CHOICE_MACROS
	decl_och_method(2)
	decl_och_method(3)
	decl_och_method(4)
	decl_och_method(5)
	decl_och_method(6)
	decl_och_method(7)
	decl_och_method(8)
	decl_och_method(9)
	decl_och_method(10)
	decl_och_method(11)
	decl_och_method(12)
	decl_och_method(13)
	decl_och_method(14)
	decl_och_method(15)

	decl_oobj_method(1)
	decl_oobj_method(2)
	decl_oobj_method(3)
	decl_oobj_method(4)
	decl_oobj_method(5)
	decl_oobj_method(6)
	decl_oobj_method(7)
	decl_oobj_method(8)
	decl_oobj_method(9)
	decl_oobj_method(10)
	decl_oobj_method(11)
	decl_oobj_method(12)
	decl_oobj_method(13)
	decl_oobj_method(14)
	decl_oobj_method(15)

#define option(t) (t*)NULL

#endif
};

#undef def_simple_input
#undef def_simple_output

#ifndef XTL_CONFIG_COMPOSITE_BUG
template <class Stream, class T>
inline void composite(Stream& stream, T& data) {
	data.composite(stream);
}
#endif

class no_refs {
 public:
 	template <class Format, class T>
	void reference(obj_input<Format>& stream, T*& data) {
		delete data;
		data=new T;
		stream.simple(*data);
	}

 	template <class Format, class T>
	void reference(obj_output<Format>& stream, T const* data) {
		stream.simple(*data);
	}
};

#if defined(for)
#undef for
#endif

#endif

#pragma warning(pop)
