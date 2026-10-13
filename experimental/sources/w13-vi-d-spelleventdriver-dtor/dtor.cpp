// Observed: _$_18VISpellEventDriver at 0x0117cf28 is 700 bytes in the pinned SLUS-20565 image and returns nothing. With (this, flags),
// in this order:
//  1. RemHead__7VILList(this+76) is called until it returns zero; each returned record e is destroyed as described below.
//  2. The same for RemHead__7VILList(this+88).
//  3. The record whose address is the word at this+32, if nonzero, is destroyed the same way (without any list removal); then the
//     word at this+36 likewise.
//  4. When the word at this+40 is nonzero, _$_11VILightning(it, 3) is called; then the same for the word at this+44.
//  5. _$_7VILList(this+88, 2) and then _$_7VILList(this+76, 2) are called.
//  6. When bit 0 of flags is set, __builtin_delete(this) is tail-called.
// Destroying a record e: when the word at e+140 is not -1, ReleaseActor__7VIScenei(word at e+16, that value) is called; then the same
// with the word at e+144 (read afterwards); then when the word at e+120 is nonzero, _$_17VIParticleEmitter(it, 3); then the same for
// the word at e+124; then when the word at e+128 is nonzero, _$_14VIMotifBlender(it, 3); finally __builtin_delete(e). The image holds
// four inline copies of this sequence.
// Inferred: +76 and +88 are two intrusive lists of event records and +32/+36 two single records; the 3 and 2 are delete-flag style
// arguments; the word at record+16 is a scene (callee name), and +140/+144 are actor ids released on it.
// Unknown: the remaining fields, the callee bodies (all imports) and the exact record type. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* ptr(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* ptr(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void* RemHead__7VILList(void* list);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void _$_11VILightning(void* lightning, int flags);
void _$_7VILList(void* list, int flags);
void __builtin_delete(void* pointer);
}

namespace {
inline __attribute__((always_inline)) void destroy_record(unsigned char* e) {
    unsigned* w = reinterpret_cast<unsigned*>(e);
    if (w[35] != 0xffffffffu) ReleaseActor__7VIScenei(ptr(w[4]), static_cast<int>(w[35]));
    if (w[36] != 0xffffffffu) ReleaseActor__7VIScenei(ptr(w[4]), static_cast<int>(w[36]));
    if (w[30] != 0) _$_17VIParticleEmitter(ptr(w[30]), 3);
    if (w[31] != 0) _$_17VIParticleEmitter(ptr(w[31]), 3);
    if (w[32] != 0) _$_14VIMotifBlender(ptr(w[32]), 3);
    __builtin_delete(e);
}
}  // namespace

extern "C" void _$_18VISpellEventDriver(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned* w = static_cast<unsigned*>(self);
    unsigned char* e;
    while ((e = static_cast<unsigned char*>(RemHead__7VILList(bytes + 76))) != nullptr) destroy_record(e);
    while ((e = static_cast<unsigned char*>(RemHead__7VILList(bytes + 88))) != nullptr) destroy_record(e);
    if (w[8] != 0) destroy_record(static_cast<unsigned char*>(ptr(w[8])));
    if (w[9] != 0) destroy_record(static_cast<unsigned char*>(ptr(w[9])));
    if (w[10] != 0) _$_11VILightning(ptr(w[10]), 3);
    if (w[11] != 0) _$_11VILightning(ptr(w[11]), 3);
    _$_7VILList(bytes + 88, 2);
    _$_7VILList(bytes + 76, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
