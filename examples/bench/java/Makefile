# XTL - the eXternalization Template Library (sample code)
# by Jose' Orlando Pereira, Universidade do Minho
# 
# jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
# 
# $Id: Makefile,v 1.2 2000/12/22 10:48:17 jop Exp $


all: Tests.class

Tests.class: Tests.java Alltests.class
	javac Tests.java

Alltests.class: Alltests.java
	javac Alltests.java

times:
	echo "\nbench/java\n" >> ../xtl.times 2>&1
	echo "java" >> ../xtl.times
	java Tests >> ../xtl.times
	java Tests >> ../xtl.times
	java Tests >> ../xtl.times

clean:
	rm -f *.class
