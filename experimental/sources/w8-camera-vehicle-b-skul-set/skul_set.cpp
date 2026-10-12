// Observed set__8SkulBoat at 0x2bf640, extent 328. It is slot 28 of the
// SkulBoat view and is called through refreshSettings__7Vehicle. It only
// stores constants into the object, with one helper call, in this order:
//   floats at +884, +896, +892, +888, then setMassAndIBody__7Vehicle(object);
//   then float and word stores (+1044 = 1, +900, +904, +1000 = 0), four
//   pointer words (+2056 and +2068 = object+2328, +2060 and +2064 =
//   object+2344), and floats at +2328..+2356, +908, +912, +916.
// The constants come from immediates and from .sdata words at 0x483030 to
// 0x48304c; they are written below as raw float bits with a decimal comment.
// Unknown: what any of the offsets mean, and why SkulBoat adds no wheels here
// (it sets +1000, the wheel count read by Vehicle Reset, to 0).
extern "C" void setMassAndIBody__7Vehicle(void* object);

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
inline void putw(unsigned char* o, unsigned off, unsigned bits) { *reinterpret_cast<AliasedWord*>(o + off) = bits; }
inline void putf(unsigned char* o, unsigned off, unsigned bits) { putw(o, off, bits); }
inline void putp(unsigned char* o, unsigned off, const unsigned char* p) {
    putw(o, off, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)));
}
}

extern "C" void set__8SkulBoat(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    putf(o, 884, 0x461c4000u);  // 10000
    putf(o, 896, 0x437a0000u);  // 250
    putf(o, 892, 0x42f00000u);  // 120
    putf(o, 888, 0x42f00000u);  // 120
    setMassAndIBody__7Vehicle(object);
    putf(o, 940, 0x42480000u);  // 50
    putf(o, 924, 0x41700000u);  // 15
    putf(o, 920, 0x41c80000u);  // 25
    putw(o, 1044, 0x1u);
    putf(o, 856, 0x43160000u);  // 150
    putf(o, 864, 0x41f00000u);  // 30
    putf(o, 868, 0x42340000u);  // 45
    putf(o, 872, 0x3f666666u);  // 0.899999976
    putf(o, 876, 0xbd4cccccu);  // -0.049999997
    putf(o, 880, 0xbdccccccu);  // -0.099999994
    putf(o, 764, 0x3f8cccccu);  // 1.0999999
    putf(o, 944, 0x42200000u);  // 40
    putf(o, 948, 0x40000000u);  // 2
    putf(o, 2344, 0x3eec4ec4u);  // 0.461538434
    putf(o, 2348, 0x47afc800u);  // 90000
    putf(o, 2352, 0x459c4000u);  // 5000
    putp(o, 2064, o + 2344);
    putp(o, 2068, o + 2328);
    putf(o, 928, 0x42480000u);  // 50
    putf(o, 932, 0x42480000u);  // 50
    putf(o, 936, 0x42480000u);  // 50
    putf(o, 860, 0x42340000u);  // 45
    putf(o, 2328, 0x3eec4ec4u);  // 0.461538434
    putf(o, 2332, 0x47afc800u);  // 90000
    putf(o, 2336, 0x459c4000u);  // 5000
    putf(o, 2340, 0x3f800000u);  // 1
    putf(o, 2356, 0x3f800000u);  // 1
    putp(o, 2056, o + 2328);
    putp(o, 2060, o + 2344);
    putf(o, 908, 0x3f800000u);  // 1
    putw(o, 900, 0x0u);
    putw(o, 904, 0x0u);
    putw(o, 1000, 0x0u);
    putf(o, 912, 0x3f800000u);  // 1
    putf(o, 916, 0x3d4cccccu);  // 0.049999997
}
