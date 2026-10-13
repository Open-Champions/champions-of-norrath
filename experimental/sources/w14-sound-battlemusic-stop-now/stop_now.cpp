// Observed: StopNow__13VIBattleMusic at 0x0106df78, 140 bytes. The object is addressed by byte offset.
//  1. scene = word at +0; if null, return. device = word at scene+19360; if null, return.
//  2. If VIGetFeatureFlags__Fv() has bit 0x100: call StopBgm__13VISoundDevicei(device, 0) and store 0 at +8.
//  3. Otherwise: id = word at +28; if id is not -1, call Stop__13VISoundDevicei(device, id) and store -1 at +28. Store 0 at +8.
// Inferred: the same layout as Stop__13VIBattleMusicb at 0x0106ded8 (scene at +0, state at +8, playback id at +28, -1 meaning none).
// This is the immediate counterpart: it stops the playback at once and returns the state to 0.
// Unknown: the state values beyond 0 and the meaning of the feature-flag bit.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned VIGetFeatureFlags__Fv();
void StopBgm__13VISoundDevicei(unsigned device, int index);
void Stop__13VISoundDevicei(unsigned device, int id);

void StopNow__13VIBattleMusic(void* music) {
    unsigned* m = static_cast<unsigned*>(music);
    unsigned scene = m[0];
    if (!scene) return;
    unsigned device = words_at(scene)[19360 / 4];
    if (!device) return;
    if (VIGetFeatureFlags__Fv() & 0x100) {
        StopBgm__13VISoundDevicei(device, 0);
        m[2] = 0;
        return;
    }
    int id = static_cast<int>(m[7]);
    if (id != -1) {
        Stop__13VISoundDevicei(device, id);
        m[7] = 0xffffffffu;
    }
    m[2] = 0;
}
}
