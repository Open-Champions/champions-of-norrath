// Host contract for addWheel__7VehiclePciffff. Not an EE plan input.
// The object keeps 4-byte-stride pointers and the host's are 8 bytes wide, so
// wheel objects are mapped where the halves line up: slot 0 = words (3, 4) =
// 0x400000003, an existing slot 1 = words (4, 3) = 0x300000004, and the
// allocation result 0x400000004 keeps word 4 in slot 0's upper half.
#include "add_wheel.cpp"

#include <cstdio>
#include <cstring>
#include <sys/mman.h>

static int alloc_calls, alloc_bytes, ctor_calls, dtor_calls;
static unsigned char* alloc_result;
static struct { unsigned char* wheel; const char* name; int k; float f0; void* vehicle; int index; } ctor;
static struct { unsigned char* object; int flag; } dtor;
static int count_at_ctor;
static unsigned char* vehicle_under_test;

extern "C" void* blockAlloc__Fi(int bytes) { ++alloc_calls; alloc_bytes = bytes; return alloc_result; }
extern "C" void __5WheelPcifP7Vehiclei(void* w, const char* name, int k, float f0, void* v, int index) {
    ++ctor_calls; ctor = {(unsigned char*)w, name, k, f0, v, index};
    std::memcpy(&count_at_ctor, vehicle_under_test + 1000, 4);
}
static void destroy(unsigned char* object, int flag) { ++dtor_calls; dtor = {object, flag}; }

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static void putw(unsigned char* o, int off, int v) { std::memcpy(o + off, &v, 4); }
static int getw(const unsigned char* o, int off) { int v; std::memcpy(&v, o + off, 4); return v; }
static float getf(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }

int main() {
    int failures = 0;
    unsigned char* hi4 = (unsigned char*)mmap((void*)0x400000000ull, 0x4000, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0);
    unsigned char* hi3 = (unsigned char*)mmap((void*)0x300000000ull, 0x4000, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0);
    if (hi4 != (void*)0x400000000ull || hi3 != (void*)0x300000000ull) { std::fprintf(stderr, "fail: cannot map the wheel pages\n"); return 2; }
    unsigned char* const wheel0 = hi4 + 3;
    unsigned char* const old1 = hi3 + 4;
    unsigned char* const fresh = hi4 + 4;
    // a virtual table for the old wheel: entry slot 8 = delta (short) then function
    alignas(8) unsigned char vtable[64];
    std::memset(vtable, 0, sizeof vtable);
    short delta = 16;
    std::memcpy(vtable + 8, &delta, 2);
    void (*fn)(unsigned char*, int) = destroy;
    std::memcpy(vtable + 12, &fn, sizeof fn);
    unsigned char* vt = vtable;
    std::memcpy(old1 + 72, &vt, sizeof vt);

    const char* const name = "wheelfrnt.vif";
    unsigned char v[2304];

    // A: empty vehicle, allocation succeeds
    std::memset(v, 0, sizeof v); vehicle_under_test = v;
    std::memset(hi4, 0, 0x4000); std::memset(hi3, 0, 0x4000); std::memcpy(old1 + 72, &vt, sizeof vt);
    alloc_result = fresh; alloc_calls = ctor_calls = dtor_calls = 0;
    putw(fresh, 388, 7);
    putw(v, 1000, 0);
    addWheel__7VehiclePciffff(v, (char*)name, 5, 2.5f, 1.0f, 2.0f, 3.0f);
    unsigned char* slot0; std::memcpy(&slot0, v + 1052, sizeof slot0);
    if (alloc_calls != 1 || alloc_bytes != 440) { std::fprintf(stderr, "fail A: alloc\n"); ++failures; }
    if (ctor_calls != 1 || ctor.wheel != fresh || ctor.name != name || ctor.k != 5 || ctor.f0 != 2.5f || ctor.vehicle != v || ctor.index != 0) { std::fprintf(stderr, "fail A: ctor\n"); ++failures; }
    if (count_at_ctor != 1) { std::fprintf(stderr, "fail A: count raised before ctor\n"); ++failures; }
    if (getw(v, 1000) != 1 || slot0 != fresh || dtor_calls != 0) { std::fprintf(stderr, "fail A: state\n"); ++failures; }
    if (getf(v, 1236) != 1.0f || getf(v, 1240) != 2.0f || getf(v, 1244) != 3.0f) { std::fprintf(stderr, "fail A: point\n"); ++failures; }
    if (getf(v, 820) != 1.0f) { std::fprintf(stderr, "fail A: driven count\n"); ++failures; }

    // B: allocation fails
    std::memset(v, 0, sizeof v); vehicle_under_test = v;
    alloc_result = nullptr; alloc_calls = ctor_calls = dtor_calls = 0;
    put(v, 820, 9.0f);
    addWheel__7VehiclePciffff(v, (char*)name, 5, 2.5f, 4.0f, 5.0f, 6.0f);
    std::memcpy(&slot0, v + 1052, sizeof slot0);
    if (ctor_calls != 0 || getw(v, 1000) != 0 || slot0 != nullptr || getf(v, 820) != 0.0f) { std::fprintf(stderr, "fail B\n"); ++failures; }
    if (getf(v, 1236) != 4.0f || getf(v, 1244) != 6.0f) { std::fprintf(stderr, "fail B: point\n"); ++failures; }

    // C: one wheel present, an old wheel in slot 1 is destroyed first
    std::memset(v, 0, sizeof v); vehicle_under_test = v;
    std::memset(hi4, 0, 0x4000);
    putw(wheel0, 388, 1); putw(fresh, 388, 0);
    putw(v, 1052, 3); putw(v, 1056, 4); putw(v, 1060, 3);   // slot 0 = wheel0, slot 1 = old1
    putw(v, 1000, 1);
    alloc_result = fresh; alloc_calls = ctor_calls = dtor_calls = 0;
    addWheel__7VehiclePciffff(v, (char*)name, 9, 0.5f, 7.0f, 8.0f, 9.0f);
    std::memcpy(&slot0, v + 1052, sizeof slot0);
    unsigned char* slot1; std::memcpy(&slot1, v + 1056, sizeof slot1);
    if (dtor_calls != 1 || dtor.object != old1 + 16 || dtor.flag != 3) { std::fprintf(stderr, "fail C: destructor\n"); ++failures; }
    if (ctor_calls != 1 || ctor.index != 1 || getw(v, 1000) != 2 || slot0 != wheel0 || slot1 != fresh) { std::fprintf(stderr, "fail C: state\n"); ++failures; }
    if (getf(v, 1248) != 7.0f || getf(v, 1252) != 8.0f || getf(v, 1256) != 9.0f) { std::fprintf(stderr, "fail C: point\n"); ++failures; }
    if (getf(v, 820) != 1.0f) { std::fprintf(stderr, "fail C: driven count %g\n", getf(v, 820)); ++failures; }
    return failures ? 1 : 0;
}
