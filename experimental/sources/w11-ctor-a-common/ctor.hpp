// Shared helpers for the w11-ctor-a constructors. Objects are addressed by byte offset because
// the image's layouts are only partly known; every offset used is stated in the function that
// uses it. EE pointers are 32-bit words, so stored pointers go through word().
#pragma once
struct Point3 { float x, y, z; };

namespace w11 {
inline unsigned word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned& u32(void* o, int off) { return *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(o) + off); }
inline int& i32(void* o, int off) { return *reinterpret_cast<int*>(static_cast<unsigned char*>(o) + off); }
inline float& f32(void* o, int off) { return *reinterpret_cast<float*>(static_cast<unsigned char*>(o) + off); }
inline unsigned short& u16(void* o, int off) { return *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(o) + off); }
inline short& i16(void* o, int off) { return *reinterpret_cast<short*>(static_cast<unsigned char*>(o) + off); }
inline unsigned char& u8(void* o, int off) { return static_cast<unsigned char*>(o)[off]; }
}  // namespace w11

extern "C" {
void game_object_construct(void* self, Point3 position, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void objectAddToRunList__FP10GameObject(void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void* P_CreateParticleSystem__Fv();
extern int frameNm;
}
