// Shared helpers for the w11-ctor-a constructors. Objects are addressed by byte offset because
// the image's layouts are only partly known; every offset used is stated in the function that
// uses it. EE pointers are 32-bit words, so stored pointers go through word().
// A host test defines W11_HOST and supplies w11_host_pointer (32-bit word -> host pointer) and
// w11_host_call_* (stand-ins for calling a function word), since a 64-bit host cannot hold a
// target pointer in a 32-bit record word.
#pragma once
struct Point3 { float x, y, z; };

#ifdef W11_HOST
extern "C" void* w11_host_pointer(unsigned address);
extern "C" unsigned w11_host_word(const void* pointer);
extern "C" int w11_host_call_int4(unsigned function, void* self, int a, int b, int c);
#endif

namespace w11 {
inline unsigned word(const void* p) {
#ifdef W11_HOST
    return p == nullptr ? 0u : w11_host_word(p);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p));
#endif
}
inline void* pointer(unsigned w) {
#ifdef W11_HOST
    return w == 0 ? nullptr : w11_host_pointer(w);
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w));
#endif
}
inline unsigned& u32(void* o, int off) { return *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(o) + off); }
inline int& i32(void* o, int off) { return *reinterpret_cast<int*>(static_cast<unsigned char*>(o) + off); }
inline float& f32(void* o, int off) { return *reinterpret_cast<float*>(static_cast<unsigned char*>(o) + off); }
inline unsigned short& u16(void* o, int off) { return *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(o) + off); }
inline short& i16(void* o, int off) { return *reinterpret_cast<short*>(static_cast<unsigned char*>(o) + off); }
inline unsigned char& u8(void* o, int off) { return static_cast<unsigned char*>(o)[off]; }

// Calls the entry at byte `slot` of the table whose address is the word at self+96 (the layout
// the skill objects use): a signed halfword adjusts `this`, the function word is at +4. The
// function takes (this + adjust, a, b, c) and returns an int.
inline int skill_call_int4(void* self, int slot, int a, int b, int c) {
    const unsigned char* entry = static_cast<const unsigned char*>(pointer(u32(self, 96))) + slot;
    const int adjust = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    void* const adjusted = static_cast<unsigned char*>(self) + adjust;
#ifdef W11_HOST
    return w11_host_call_int4(function, adjusted, a, b, c);
#else
    return reinterpret_cast<int (*)(void*, int, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, a, b, c);
#endif
}
}  // namespace w11

extern "C" {
void game_object_construct(void* self, Point3 position, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void projectile_construct(void* self, Point3 position, void* owner, void* map)
    __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void objectAddToRunList__FP10GameObject(void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void* P_CreateParticleSystem__Fv();
float icos__Fi(int angle);
float isin__Fi(int angle);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void lightAdd__FP5Light(void* light);
extern int frameNm;
}
