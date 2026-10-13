// Observed: ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent at 0x0117f858 is 368
// bytes in the pinned SLUS-20565 image. Arguments: (this, node). Always returns 0. ev = the word at
// node+116 (the script record); the blender is the word at node+128; start = the 64-bit value at
// node+176 (the time the event began).
//   1. t = GetTime__7VIScene(the word at this+20). elapsed = the low 32 bits of (t - start), as a signed
//      int.
//   2. When the int at ev+12 is below elapsed (the duration has run out): f = Factor__C14VIMotif
//      Blender(blender); if f differs from the float at ev+20, SetFactor__14VIMotifBlenderf(blender,
//      that float).
//      Otherwise: r = (float)(GetTime(scene) - start) (a second time read, 64-bit to float through
//      __floatdisf) divided by (float)(the int at ev+12); v = the float at node+156 + (the float at ev+20
//      minus that start float) * r; clamped to [0, 1] with max.s against 0.0f then min.s against 1.0f;
//      SetFactor__14VIMotifBlenderf(blender, v).
//   3. When Motif__14VIMotifBlender(blender) is non-null it returns 0. Otherwise it removes the node:
//      Remove__7VILNode(node); when the word at node+140 is not -1, ReleaseActor__7VIScenei(the word at
//      node+16, that value); the same for node+144; when the word at node+120 is non-zero
//      _$_17VIParticleEmitter(it, 3); the same for node+124; when the word at node+128 is non-zero
//      _$_14VIMotifBlender(it, 3); then __builtin_delete(node). Returns 0.
// Inferred: the per-frame step of an emitter-motif event (type 6): it fades the blend factor from the
// start factor stored at node+156 to the target at ev+20 over ev+12 time units, holds the target once
// the time is up, and deletes the event when the blender no longer has a motif.
// Unknown: the time unit, why the first test compares a truncated 32-bit difference while the second
// branch uses the full 64-bit one, and what a null Motif means (finished blend). The compiled bodies use the same max.s and min.s through inline assembly, so zero durations and NaN behave as in the image. Nothing here ran on
// the EE.
extern "C" {
long long GetTime__7VIScene(void* scene);
float Factor__C14VIMotifBlender(void* blender);
void SetFactor__14VIMotifBlenderf(void* blender, float factor);
void* Motif__14VIMotifBlender(void* blender);
void Remove__7VILNode(void* node);
void ReleaseActor__7VIScenei(void* scene, int actor);
void _$_17VIParticleEmitter(void* emitter, int flags);
void _$_14VIMotifBlender(void* blender, int flags);
void __builtin_delete(void* pointer);

// The image uses the R5900 max.s and min.s instructions, whose results differ from a compare-and-select
// for infinities and NaN (the duration may be zero); they are issued directly.
static inline float floatMax(float a, float b) {
    float r;
    __asm__("max.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
static inline float floatMin(float a, float b) {
    float r;
    __asm__("min.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}

int ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent(unsigned char* self, unsigned char* node) {
    long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    unsigned char* ev = *reinterpret_cast<unsigned char**>(node + 116);
    int elapsed = static_cast<int>(now - *reinterpret_cast<long long*>(node + 176));
    if (*reinterpret_cast<int*>(ev + 12) < elapsed) {
        float factor = Factor__C14VIMotifBlender(*reinterpret_cast<void**>(node + 128));
        float target = *reinterpret_cast<float*>(ev + 20);
        if (factor != target) {
            SetFactor__14VIMotifBlenderf(*reinterpret_cast<void**>(node + 128), target);
        }
    } else {
        long long later = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
        float ratio = static_cast<float>(later - *reinterpret_cast<long long*>(node + 176)) / static_cast<float>(*reinterpret_cast<int*>(ev + 12));
        float start = *reinterpret_cast<float*>(node + 156);
        float value = start + (*reinterpret_cast<float*>(ev + 20) - start) * ratio;
        value = floatMin(floatMax(value, 0.0f), 1.0f);
        SetFactor__14VIMotifBlenderf(*reinterpret_cast<void**>(node + 128), value);
    }
    if (Motif__14VIMotifBlender(*reinterpret_cast<void**>(node + 128)) != 0) {
        return 0;
    }
    Remove__7VILNode(node);
    if (*reinterpret_cast<int*>(node + 140) != -1) {
        ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 140));
    }
    if (*reinterpret_cast<int*>(node + 144) != -1) {
        ReleaseActor__7VIScenei(*reinterpret_cast<void**>(node + 16), *reinterpret_cast<int*>(node + 144));
    }
    if (*reinterpret_cast<void**>(node + 120) != 0) {
        _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 120), 3);
    }
    if (*reinterpret_cast<void**>(node + 124) != 0) {
        _$_17VIParticleEmitter(*reinterpret_cast<void**>(node + 124), 3);
    }
    if (*reinterpret_cast<void**>(node + 128) != 0) {
        _$_14VIMotifBlender(*reinterpret_cast<void**>(node + 128), 3);
    }
    __builtin_delete(node);
    return 0;
}
}
