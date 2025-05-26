all: cnum_calculator
libcnum: complexnumber.o expressionparser.o fractionnum.o
	g++ -shared -fPIC $^ -o libcnum.so

cnum_calculator: cnum_calculator.o libcnum libcnum_test
	g++ cnum_calculator.o -L. -lcnum -Wl,-rpath='$$ORIGIN' -o cnum_calculator

libcnum_test: libcnum_test.o libcnum
	g++ libcnum_test.o -L. -lcnum -Wl,-rpath='$$ORIGIN' -o libcnum_test
	./libcnum_test

libcnum_test.o: libcnum_test.cpp
	g++ -c libcnum_test.cpp
cnum_calculator.o: cnum_calculator.cpp
	g++ -c cnum_calculator.cpp


complexnumber.o: src/complexnumber.cpp
	g++ -fPIC -c src/complexnumber.cpp

expressionparser.o: src/expressionparser.cpp
	g++ -fPIC -c src/expressionparser.cpp

fractionnum.o: src/fractionnum.cpp
	g++ -fPIC -c src/fractionnum.cpp

clean:
	rm -rf fractionnum.o expressionparser.o complexnumber.o libcnum_test.o libcnum_test cnum_calculator.o cnum_calculator libcnum.so

install_libcnum: libcnum
	cp libcnum.so /usr/lib64/
	cp include/libcomplexnumber.h /usr/include/

install: cnum_calculator install_libcnum
	cp cnum_calculator /usr/bin/

uninstall:
	rm -rf /usr/bin/cnum_calculator
	rm -rf /usr/lib64/libcnum.so
	rm -rf /usr/include/libcomplexnumber.h