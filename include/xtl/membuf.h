/*
 * Memory buffer driver for XTL
 *
 * Copyright (C) 2000 Allan Rae, allan.rae@mailbox.uq.edu.au
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
 */

#pragma once

#include <xtl/config.h>
#include <xtl/objio.h>

class mem_buffer {
 private:
 	char *buffer, *ptr, *lim;

 public:
	class buffer_overflow_error: public std::overflow_error {
	 public:
	 	std::size_t bytes_left;
		std::size_t bytes_needed;

		buffer_overflow_error(std::size_t left, std::size_t needed):
		std::overflow_error("XTL mem_buffer overflow"),
			bytes_left(left),bytes_needed(needed) {}
	};

 	mem_buffer(void *buf, std::size_t size):
		buffer((char*)buf),ptr((char*)buf),lim(((char*)buf)+size) {}

	inline void read(char* inPtr, std::size_t size)
		{memcpy(inPtr, require(size), size);}
	inline void write(char const* inPtr, std::size_t size)
		{memcpy(desire(size), inPtr, size);}

	inline void* require(std::size_t size) {
		char* aux=ptr;
		if ((ptr+=size)>lim)
			throw buffer_overflow_error((std::size_t)(lim-aux),size);
		return aux;
	}
	inline void* desire(std::size_t size)
		{return require(size);}
	inline void flush()
		{}

	inline void unrequire(std::size_t n)
		{ptr-=n;}
	inline void undesire(std::size_t n)
		{unrequire(n);}
	inline void rewind()
		{ptr=buffer;}
	inline std::size_t size() const
	{return (std::size_t)(ptr-buffer);}
	inline char* data() const
		{return buffer;}
	inline char* ptrdata() const
	{ return ptr; }
	inline std::size_t ptrsize() const
	{ return (std::size_t)(lim-ptr); }

	template <class Format>
	inline void composite(obj_input<Format>& stream) {
		std::size_t size, idx;
		stream.array(buffer, size).simple(idx);
		ptr=buffer+idx;
		lim=buffer+size;
	}

	template <class Format>
	inline void composite(obj_output<Format>& stream) {
		stream.array(buffer, lim-buffer).simple((std::size_t)(ptr-buffer));
	}

};
