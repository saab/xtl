# XTL - the eXternalization Template Library (sample code)
# by Jose' Orlando Pereira, Universidade do Minho
# 
# jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
#
# $Id: Makefile,v 1.2 2000/12/22 10:48:17 jop Exp $


all:
	make -C xtl
	make -C vxtl
	make -C memcpy
	make -C rpcgen
	make -C java

times: xtl.times
	grep -v input xtl.times > times
	rm -f xtl.times

xtl.times:
	date > xtl.times
	make -C memcpy times
	make -C rpcgen times
	make -C xtl times
	make -C vxtl times
	make -C java times

clean:
	make -C xtl clean
	make -C rpcgen clean
	make -C memcpy clean
	make -C vxtl clean
	make -C java clean
	rm -f xtl.times times

