/*
 * XTL - the eXternalization Template Library (sample code)
 * by Jose' Orlando Pereira, Universidade do Minho
 *
 * jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
 *
 * $Id: compare.cpp,v 1.2 2000/12/22 10:48:18 jop Exp $
 */


#include <iostream>
#include <xtl/objio.h>

#include "../alltests.h"

void compare(const all_tests& to, const all_tests& ti) {
	std::cout << (to==ti?"PASSED":"Failed") << std::endl;
}

