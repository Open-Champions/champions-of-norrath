// Observed: Process__14VIEffectVolumef at 0x01147240 is 296 bytes. Argument: the object (V) and a float that is not read (the code
// ignores its own argument and takes time from a timer). It calls GetTime__7VITimer, a 64-bit result T, and forms
// dt = (float)(int)(T - the doubleword at V+0) * the float 0x3a83126e (about 0.001). If the word at V+72 is non-zero it calls
// AllocParticles__14VIEffectVolume(V) and clears that word. When dt < 0x3c888888 (about 1/60) it returns 0 and does nothing else (the
// doubleword at V+0 keeps its old value). Otherwise it stores T in the doubleword at V+0, limits dt to 0x3dcccccc (about 0.1) when it
// is larger, and dispatches on the word at V+36: type 1 calls ProcessFallingLeaves__14VIEffectVolumef(V, dt), 2 ProcessLightShafts,
// 3 ProcessGroundFog and 4 ProcessDustStorm, each with the same (V, dt); type 0 and any value of 5 or more call nothing. It returns 0.
// Inferred: dt is seconds since the last processed frame; the 1/60 test skips updates that come sooner than one 60 Hz frame; the four
// processes are the effect-volume kinds the dispatch table names. The doubleword at V+0 is a last-update time stamp in timer units.
// Unknown: the timer unit beyond the 0.001 scale, and what type 0 does.
extern "C" {
long long GetTime__7VITimer();
void AllocParticles__14VIEffectVolume(void* self);
void ProcessFallingLeaves__14VIEffectVolumef(void* self, float dt);
void ProcessLightShafts__14VIEffectVolumef(void* self, float dt);
void ProcessGroundFog__14VIEffectVolumef(void* self, float dt);
void ProcessDustStorm__14VIEffectVolumef(void* self, float dt);

int Process__14VIEffectVolumef(unsigned char* self, float) {
    long long now = GetTime__7VITimer();
    float dt = static_cast<float>(static_cast<int>(now - *reinterpret_cast<long long*>(self))) * 0x1.0624dcp-10f;
    if (*reinterpret_cast<unsigned*>(self + 72) != 0) {
        AllocParticles__14VIEffectVolume(self);
        *reinterpret_cast<unsigned*>(self + 72) = 0;
    }
    if (0x1.111110p-6f <= dt) {
        *reinterpret_cast<long long*>(self) = now;
        if (0x1.99999800p-4f < dt) dt = 0x1.99999800p-4f;
        switch (*reinterpret_cast<unsigned*>(self + 36)) {
        case 1: ProcessFallingLeaves__14VIEffectVolumef(self, dt); break;
        case 2: ProcessLightShafts__14VIEffectVolumef(self, dt); break;
        case 3: ProcessGroundFog__14VIEffectVolumef(self, dt); break;
        case 4: ProcessDustStorm__14VIEffectVolumef(self, dt); break;
        default: break;
        }
    }
    return 0;
}
}
