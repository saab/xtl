/*
 * Some ugly macros for XTL. Keep out!
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
 * $Id: macros.h,v 1.2 2000/12/22 10:48:18 jop Exp $
 */

#ifndef __XTL_MACROS
#define __XTL_MACROS

// Extremely yucky pre-processor wizardry. Don't look. Just pretend it
// is not here. :-)

#define tparam(i) class T##i

#define ci_param(i) T##i& val##i
#define co_param(i) const T##i& val##i
#define c_case(i) case i:\
			simple(val##i);\
			break;

#define o_param(i) T##i* tag##i
#define o_case(i) case i:\
			reference(tag##i);\
			ptr=(Base*)tag##i;\
			break;
#define o_elseif(i) else if (typeid(*ptr)==typeid(T##i)) { \
			simple(i); \
			tag##i=(T##i*)ptr; \
			reference(tag##i); \
		}

#define tparam_1 tparam(0)
#define tparam_2 tparam(0), tparam(1)
#define tparam_3 tparam_2, tparam(2)
#define tparam_4 tparam_3, tparam(3)
#define tparam_5 tparam_4, tparam(4)
#define tparam_6  tparam_5,  tparam(5)
#define tparam_7  tparam_6,  tparam(6)
#define tparam_8  tparam_7,  tparam(7)
#define tparam_9  tparam_8,  tparam(8)
#define tparam_10 tparam_9,  tparam(9)
#define tparam_11 tparam_10, tparam(10)
#define tparam_12 tparam_11, tparam(11)
#define tparam_13 tparam_12, tparam(12)
#define tparam_14 tparam_13, tparam(13)
#define tparam_15 tparam_14, tparam(14)
#define tparam_16 tparam_15, tparam(15)
#define tparam_17 tparam_16, tparam(16)

#define X_tparam(i) tparam_##i

#define ci_param_2 ci_param(0), ci_param(1)
#define ci_param_3 ci_param_2, ci_param(2)
#define ci_param_4 ci_param_3, ci_param(3)
#define ci_param_5 ci_param_4, ci_param(4)
#define ci_param_6  ci_param_5,  ci_param(5)
#define ci_param_7  ci_param_6,  ci_param(6)
#define ci_param_8  ci_param_7,  ci_param(7)
#define ci_param_9  ci_param_8,  ci_param(8)
#define ci_param_10 ci_param_9,  ci_param(9)
#define ci_param_11 ci_param_10, ci_param(10)
#define ci_param_12 ci_param_11, ci_param(11)
#define ci_param_13 ci_param_12, ci_param(12)
#define ci_param_14 ci_param_13, ci_param(13)
#define ci_param_15 ci_param_14, ci_param(14)
#define ci_param_16 ci_param_15, ci_param(15)
#define ci_param_17 ci_param_16, ci_param(16)

#define X_ci_param(i) ci_param_##i

#define co_param_2 co_param(0), co_param(1)
#define co_param_3 co_param_2, co_param(2)
#define co_param_4 co_param_3, co_param(3)
#define co_param_5 co_param_4, co_param(4)
#define co_param_6  co_param_5,  co_param(5)
#define co_param_7  co_param_6,  co_param(6)
#define co_param_8  co_param_7,  co_param(7)
#define co_param_9  co_param_8,  co_param(8)
#define co_param_10 co_param_9,  co_param(9)
#define co_param_11 co_param_10, co_param(10)
#define co_param_12 co_param_11, co_param(11)
#define co_param_13 co_param_12, co_param(12)
#define co_param_14 co_param_13, co_param(13)
#define co_param_15 co_param_14, co_param(14)
#define co_param_16 co_param_15, co_param(15)
#define co_param_17 co_param_16, co_param(16)

#define X_co_param(i) co_param_##i

#define c_case_2 c_case(0) c_case(1)
#define c_case_3 c_case_2 c_case(2)
#define c_case_4 c_case_3 c_case(3)
#define c_case_5 c_case_4 c_case(4)
#define c_case_6  c_case_5  c_case(5)
#define c_case_7  c_case_6  c_case(6)
#define c_case_8  c_case_7  c_case(7)
#define c_case_9  c_case_8  c_case(8)
#define c_case_10 c_case_9  c_case(9)
#define c_case_11 c_case_10 c_case(10)
#define c_case_12 c_case_11 c_case(11)
#define c_case_13 c_case_12 c_case(12)
#define c_case_14 c_case_13 c_case(13)
#define c_case_15 c_case_14 c_case(14)
#define c_case_16 c_case_15 c_case(15)
#define c_case_17 c_case_16 c_case(16)

#define X_c_case(i) c_case_##i

#define o_param_1 o_param(0)
#define o_param_2 o_param(0), o_param(1)
#define o_param_3 o_param_2, o_param(2)
#define o_param_4 o_param_3, o_param(3)
#define o_param_5 o_param_4, o_param(4)
#define o_param_6  o_param_5,  o_param(5)
#define o_param_7  o_param_6,  o_param(6)
#define o_param_8  o_param_7,  o_param(7)
#define o_param_9  o_param_8,  o_param(8)
#define o_param_10 o_param_9,  o_param(9)
#define o_param_11 o_param_10, o_param(10)
#define o_param_12 o_param_11, o_param(11)
#define o_param_13 o_param_12, o_param(12)
#define o_param_14 o_param_13, o_param(13)
#define o_param_15 o_param_14, o_param(14)
#define o_param_16 o_param_15, o_param(15)
#define o_param_17 o_param_16, o_param(16)

#define X_o_param(i) o_param_##i

#define o_case_1 o_case(0)
#define o_case_2 o_case(0)  o_case(1)
#define o_case_3 o_case_2 o_case(2)
#define o_case_4 o_case_3 o_case(3)
#define o_case_5 o_case_4 o_case(4)
#define o_case_6  o_case_5  o_case(5)
#define o_case_7  o_case_6  o_case(6)
#define o_case_8  o_case_7  o_case(7)
#define o_case_9  o_case_8  o_case(8)
#define o_case_10 o_case_9  o_case(9)
#define o_case_11 o_case_10 o_case(10)
#define o_case_12 o_case_11 o_case(11)
#define o_case_13 o_case_12 o_case(12)
#define o_case_14 o_case_13 o_case(13)
#define o_case_15 o_case_14 o_case(14)
#define o_case_16 o_case_15 o_case(15)
#define o_case_17 o_case_16 o_case(16)

#define X_o_case(i) o_case_##i

#define o_elseif_1 
#define o_elseif_2 o_elseif(1)
#define o_elseif_3 o_elseif_2 o_elseif(2)
#define o_elseif_4 o_elseif_3 o_elseif(3)
#define o_elseif_5 o_elseif_4 o_elseif(4)
#define o_elseif_6  o_elseif_5  o_elseif(5)
#define o_elseif_7  o_elseif_6  o_elseif(6)
#define o_elseif_8  o_elseif_7  o_elseif(7)
#define o_elseif_9  o_elseif_8  o_elseif(8)
#define o_elseif_10 o_elseif_9  o_elseif(9)
#define o_elseif_11 o_elseif_10 o_elseif(10)
#define o_elseif_12 o_elseif_11 o_elseif(11)
#define o_elseif_13 o_elseif_12 o_elseif(12)
#define o_elseif_14 o_elseif_13 o_elseif(13)
#define o_elseif_15 o_elseif_14 o_elseif(14)
#define o_elseif_16 o_elseif_15 o_elseif(15)
#define o_elseif_17 o_elseif_16 o_elseif(16)

#define X_o_elseif(i) o_elseif_##i

#define decl_ich_method(i) \
	template <class Discr, X_tparam(i)> \
	inline obj_input& choices(Discr& discr, X_ci_param(i)) { \
		simple(discr); \
		switch(discr) { \
			X_c_case(i) \
		 default: \
		 	throw std::range_error("XTL invalid choice"); \
		}; \
		return *this; \
	}

#define decl_och_method(i) \
	template <class Discr, X_tparam(i)> \
	inline obj_output& choices(const Discr& discr, X_co_param(i)) { \
		simple(discr); \
		switch(discr) { \
			X_c_case(i) \
		 default: \
		 	throw std::range_error("XTL invalid choice"); \
		}; \
		return *this; \
	}

// frsj1: added null-pointer case.
#define decl_iobj_method(i) \
	template <class Base, X_tparam(i)> \
	inline obj_input& object(Base*& ptr, X_o_param(i)) { \
		int discr; \
		simple(discr); \
		switch(discr) { \
		 case -1: \
			ptr = 0; \
			break; \
			X_o_case(i) \
		 default: \
		 	throw std::range_error("XTL invalid choice"); \
		}; \
		return *this; \
	}

// frsj1: added null-pointer case.
#define decl_oobj_method(i) \
	template <class Base, X_tparam(i)> \
	inline obj_output& object(Base* ptr, X_o_param(i))  { \
		if (ptr == 0) \
			simple(-1); \
		else if (typeid(*ptr)==typeid(T0)) { \
			simple(0); \
			tag0=(T0*)ptr; \
			reference(tag0); \
		} X_o_elseif(i) \
		else \
			throw std::range_error("XTL invalid choice"); \
		return *this; \
	} 

#endif
