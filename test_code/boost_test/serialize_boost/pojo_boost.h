#ifndef _POJO_BOOST_H_
#define _POJO_BOOST_H_
#include <string>
using namespace std;
class basic_pojo_boost {
public :
	char char8;
	unsigned char uchar8;
	short short16;
	unsigned short ushort16;
	int int32;
	unsigned int uint32;
	long long32;
	unsigned long ulong32;
	float float32;
	double double64;
	bool bool8;
};

class complex_pojo_boost {
public :
	string string_stl;
	basic_pojo_boost basic_class;
};

namespace boost {
	namespace serialization {
		template<class Archive>
		void serialize(Archive & ar, basic_pojo_boost & bp, const unsigned int version) {
			ar & bp.bool8;
			ar & bp.char8;
			ar & bp.double64;
			ar & bp.float32;
			ar & bp.int32;
			ar & bp.long32;
			ar & bp.short16;
			ar & bp.uchar8;
			ar & bp.uint32;
			ar & bp.ulong32;
			ar & bp.ushort16;
		}

		template<class Archive>
		void serialize(Archive & ar, complex_pojo_boost & cp, const unsigned int version) {
			ar & cp.basic_class;
			ar & cp.string_stl;
		}
	}
}

#endif