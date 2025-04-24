/*
 * XTL - the eXternalization Template Library (sample code)
 * by Jose' Orlando Pereira, Universidade do Minho
 *
 * jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
 *
 * $Id: tests.cpp,v 1.2 2000/12/22 10:48:17 jop Exp $
 */

#include <xtl/config.h>

int main() {
	char buf1[400*1000], *ptr;
	char buf2[400];
	for(int j=0;j<100;j++) {
		ptr=buf1;
		for(int i=0;i<1000;i++,ptr+=380)
			memcpy(ptr, buf2, 380);
	}
	return 0;
}
