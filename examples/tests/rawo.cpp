/*
 * XTL - the eXternalization Template Library (sample code)
 * by Jose' Orlando Pereira, Universidade do Minho
 *
 * jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
 *
 * $Id: rawo.cpp,v 1.2 2000/12/22 10:48:18 jop Exp $
 */


#include <iostream>

#include <xtl/objio.h>

#include "../alltests.h"

void output(char buf[], const all_tests& to) {
	mem_buffer mbo(buf, 1000);
	raw_format<mem_buffer> rfo(mbo);
	obj_output<raw_format<mem_buffer> > os(rfo);

	os.composite(to);
}
