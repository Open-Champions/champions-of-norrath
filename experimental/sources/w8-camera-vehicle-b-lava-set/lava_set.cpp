// Observed set__11LavaTractor at 0x284918, extent 620. It is slot 28 of the
// LavaTractor view and is called through refreshSettings__7Vehicle. It only
// stores constants into the object, with two helper calls, in this order:
//   floats at +884, +888, +896, +892, then setMassAndIBody__7Vehicle(object);
//   ~25 float and word stores (including +1000 = 0 and +900, +904, +1044 = 0);
//   four addWheel__7VehiclePciffff calls, each with flag 1 and four floats:
//     "wheelfrnt.vif"  1.0,  31.0, -89.0, -19.0
//     "wheelbk.vif"    0.0,  47.0,  63.0, -19.0
//     "wheelbk.vif"    0.0, -47.0,  63.0, -19.0
//     "wheelfrnt.vif"  1.0, -31.0, -89.0, -19.0
//   then floats at +2328..+2356, +908, +912, +916 and four pointer words:
//   +2056 and +2068 = object+2328, +2060 and +2064 = object+2344.
// The constants come from immediates and from .sdata/.lit4 words near
// 0x4823d0 to 0x48240c; they are written below as raw float bits with a
// decimal comment. Strings are at 0x476d30 and 0x476d40; here they are private
// copies. Unknown: what any of the offsets mean, and addWheel's return type.
extern "C" void setMassAndIBody__7Vehicle(void* object);
extern "C" unsigned addWheel__7VehiclePciffff(void* object, char* model, int flag, float a, float b, float c, float d);

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
inline void putw(unsigned char* o, unsigned off, unsigned bits) { *reinterpret_cast<AliasedWord*>(o + off) = bits; }
inline void putf(unsigned char* o, unsigned off, unsigned bits) { putw(o, off, bits); }
inline void putp(unsigned char* o, unsigned off, const unsigned char* p) {
    putw(o, off, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)));
}
}
namespace {
char wheel_front[] = "wheelfrnt.vif";
char wheel_back[] = "wheelbk.vif";
}

extern "C" void set__11LavaTractor(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    putf(o, 884, 0x44fca000u);  // 2021
    putf(o, 888, 0x43180000u);  // 152
    putf(o, 896, 0x437a0000u);  // 250
    putf(o, 892, 0x42b00000u);  // 88
    setMassAndIBody__7Vehicle(object);
    putf(o, 936, 0x42e20000u);  // 113
    putf(o, 924, 0x41700000u);  // 15
    putf(o, 920, 0x41c80000u);  // 25
    putf(o, 856, 0x42d20000u);  // 105
    putf(o, 860, 0x42200000u);  // 40
    putf(o, 864, 0x41d00000u);  // 26
    putf(o, 868, 0x41db6fb3u);  // 27.4295406
    putf(o, 872, 0x3f666666u);  // 0.899999976
    putf(o, 876, 0xbfcef26au);  // -1.61677289
    putf(o, 880, 0xc0bda025u);  // -5.92579889
    putf(o, 764, 0x4018f42fu);  // 2.38990378
    putf(o, 944, 0x4182285au);  // 16.2697029
    putf(o, 948, 0x40000000u);  // 2
    putf(o, 928, 0x429e0000u);  // 79
    putf(o, 932, 0x42de0000u);  // 111
    putf(o, 940, 0x42c00000u);  // 96
    putw(o, 900, 0x0u);
    putw(o, 904, 0x0u);
    putw(o, 1044, 0x0u);
    putw(o, 1000, 0x0u);
    addWheel__7VehiclePciffff(object, wheel_front, 1, 1.0f, 31.0f, -89.0f, -19.0f);
    addWheel__7VehiclePciffff(object, wheel_back, 1, 0.0f, 47.0f, 63.0f, -19.0f);
    addWheel__7VehiclePciffff(object, wheel_back, 1, 0.0f, -47.0f, 63.0f, -19.0f);
    addWheel__7VehiclePciffff(object, wheel_front, 1, 1.0f, -31.0f, -89.0f, -19.0f);
    putf(o, 2340, 0x3f800000u);  // 1
    putf(o, 2332, 0x465ea000u);  // 14248
    putf(o, 916, 0x3be568a5u);  // 0.00700099999
    putf(o, 2336, 0x44fa8000u);  // 2004
    putf(o, 2344, 0x3ed20d20u);  // 0.410256386
    putf(o, 2348, 0x46664400u);  // 14737
    putf(o, 2352, 0x45053000u);  // 2131
    putf(o, 2356, 0x3f80e475u);  // 1.00697196
    putp(o, 2064, o + 2344);
    putp(o, 2068, o + 2328);
    putf(o, 908, 0x40fcfbc7u);  // 7.90573454
    putf(o, 912, 0x3f2205ffu);  // 0.632903993
    putf(o, 2328, 0x3ed20d20u);  // 0.410256386
    putp(o, 2056, o + 2328);
    putp(o, 2060, o + 2344);
}
