// Observed addWheel__7VehiclePciffff at 0x00271b20, 364 bytes. Arguments: the
// vehicle, a name string, an int k, then four floats f0..f3 (f0 first).
// With n the signed word at vehicle+1000 (the wheel count) and slot n the
// pointer at vehicle + 1052 + 4 * n:
//   1. If slot n already holds a wheel, its virtual function in table slot 8
//      (halfword delta, then function; the wheel's table pointer is at +72) is
//      called with the wheel adjusted by the delta and the flag 3.
//   2. The Point3 at vehicle + 1236 + 12 * n is set to (f1, f2, f3).
//   3. A new object of 440 bytes comes from blockAlloc__Fi. When it is not
//      null, the word at +1000 is incremented and the object is constructed
//      with __5WheelPcifP7Vehiclei(object, name, k, f0, vehicle, n).
//   4. Slot n takes the pointer (null when the allocation failed). The float at
//      +820 becomes 0.0 and then gains 1.0 for each of the first +1000 wheels
//      whose word at +388 is non-zero.
// Note that the Point3 is written and the old wheel destroyed before the
// allocation result is known, and the count is read again after construction.
// What the four floats mean is not established here. Nothing is returned.
// Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef void (*VirtualFunction)(unsigned char*, int);
typedef VirtualFunction AliasedFunction __attribute__((may_alias));

extern "C" {
void* blockAlloc__Fi(int bytes);
void __5WheelPcifP7Vehiclei(void* wheel, const char* name, int k, float f0, void* vehicle, int index);
}

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline unsigned char*& slot(unsigned char* vehicle, int index) {
    return *reinterpret_cast<AliasedPointer*>(vehicle + 1052 + 4 * index);
}
}

extern "C" void addWheel__7VehiclePciffff(unsigned char* vehicle, char* name, int k, float f0, float f1, float f2, float f3) {
    int n = word(vehicle, 1000);
    unsigned char* const old = slot(vehicle, n);
    if (old != 0) {
        unsigned char* entry = *reinterpret_cast<AliasedPointer*>(old + 72) + 8;
        (*reinterpret_cast<AliasedFunction*>(entry + 4))(old + *reinterpret_cast<AliasedShort*>(entry), 3);
        n = word(vehicle, 1000);
    }
    real(vehicle, 1236 + 12 * n + 0) = f1;
    real(vehicle, 1236 + 12 * n + 4) = f2;
    real(vehicle, 1236 + 12 * n + 8) = f3;
    unsigned char* const wheel = static_cast<unsigned char*>(blockAlloc__Fi(440));
    if (wheel != 0) {
        const int index = word(vehicle, 1000);
        word(vehicle, 1000) = index + 1;
        __5WheelPcifP7Vehiclei(wheel, name, k, f0, vehicle, index);
    }
    slot(vehicle, n) = wheel;
    const int count = word(vehicle, 1000);
    word(vehicle, 820) = 0;
    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            if (word(slot(vehicle, i), 388) != 0) real(vehicle, 820) = real(vehicle, 820) + 1.0f;
        }
    }
}
