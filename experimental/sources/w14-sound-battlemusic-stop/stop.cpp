// Observed: Stop__13VIBattleMusicb at 0x0106ded8, 160 bytes. Words are indexed by byte offset from the object (music).
//  1. scene = word at +0; if it is null, return. device = word at scene+19360; if that is null, return.
//  2. If VIGetFeatureFlags__Fv() has bit 0x100: store 0 at music+8, then continue into StopBgm__13VISoundDevicei(device, 0)
//     as the last action (the result is not used).
//  3. Otherwise: store the argument (a bool) at music+20; if the word at music+28 equals -1, store 0 at music+8, else store 2.
// Inferred: music+0 is the owning scene and scene+19360 its sound device (Init__13VIBattleMusicP7VIScene stores the scene at +0;
// Play and Process read the same slot). music+8 is a state value: 0 idle, 2 fading out, 1 and 3 appear in Play/Process.
// music+28 is a playback id (-1 when none, as Init and StopNow set it). The bool at +20 selects the fade rate that Process uses.
// Unknown: the scene layout around +19360, the feature-flag bit meaning and what the other branch of that flag selects.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned VIGetFeatureFlags__Fv();
void StopBgm__13VISoundDevicei(unsigned device, int index);

void Stop__13VIBattleMusicb(void* music, bool immediate) {
    unsigned* m = static_cast<unsigned*>(music);
    unsigned scene = m[0];
    if (!scene) return;
    unsigned device = words_at(scene)[19360 / 4];
    if (!device) return;
    if (VIGetFeatureFlags__Fv() & 0x100) {
        m[2] = 0;
        StopBgm__13VISoundDevicei(device, 0);
        return;
    }
    m[5] = immediate;
    m[2] = (m[7] == 0xffffffffu) ? 0 : 2;
}
}
