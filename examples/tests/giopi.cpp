/*
 * XTL - the eXternalization Template Library (sample code)
 * by Jose' Orlando Pereira, Universidade do Minho
 *
 * jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
 *
 * $Id: giopi.cpp,v 1.2 2000/12/22 10:48:18 jop Exp $
 */


#include <iostream>

#include <xtl/objio.h>
#include <xtl/giop.h>

#include "../alltests.h"

void input(char buf[], all_tests& ti) {
	mem_buffer mbi(buf, 1000);
	GIOP_format<mem_buffer> rfi(mbi);
	obj_input<GIOP_format<mem_buffer> > is(rfi);

	is.composite(ti);
}
