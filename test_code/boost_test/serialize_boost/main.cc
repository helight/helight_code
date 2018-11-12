#include "pojo_boost.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

size_t siz;
vector<complex_pojo_boost *> pojos_boost;

void test2() {
	siz = 100000;
	pojos_boost.resize(siz);

	complex_pojo_boost * cp_boost = NULL;

	for (size_t i = 0; i < siz; i++) {
		cp_boost = new complex_pojo_boost;
		cp_boost->basic_class.char8 = 0;
		cp_boost->basic_class.uchar8 = 1;
		cp_boost->basic_class.short16 = 2;
		cp_boost->basic_class.ushort16 = 3;
		cp_boost->basic_class.int32 = i;
		cp_boost->basic_class.uint32 = 5;
		cp_boost->basic_class.long32 = 6;
		cp_boost->basic_class.ulong32 = 7;
		cp_boost->basic_class.float32 = 8.0;
		cp_boost->basic_class.double64 = 9.0;
		cp_boost->basic_class.bool8 = true;
		cp_boost->string_stl = "default";
		pojos_boost[i] = cp_boost;
		cp_boost = NULL;
	}
}

void test1() {
	ofstream fout("boost_test.dat");
	boost::archive::binary_oarchive oa(fout);
	for(size_t i = 0; i < siz; i++) {
		oa << *(pojos_boost[i]);
	}
	fout.close();
}

void test() {
	ifstream fin("boost_test.dat");
	boost::archive::binary_iarchive ia(fin);

	complex_pojo_boost * cp = NULL;
	for (size_t i = 0; i < siz; i++) {
		cp = new complex_pojo_boost;
		ia >> *cp;
		assert(cp->basic_class.char8 == 0);
        std::cout << "cp_boost->basic_class.int32 : " << cp->basic_class.int32  << std::endl;
		delete cp;
		cp = NULL;
	}
	fin.close();
}

int main()
{
    test2();
    test1();
    test();
	return 0;
}
