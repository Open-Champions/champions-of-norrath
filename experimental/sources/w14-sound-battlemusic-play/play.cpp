// Observed: Play__13VIBattleMusici at 0x0106d920, 368 bytes. Words are indexed by byte offset from the object (music); argument is
// an int (track). It returns 0 except in one case noted in step 3. In order:
//  1. scene = word at +0; if null return 0. device = word at scene+19360; if null return 0.
//  2. If VIGetFeatureFlags__Fv() has bit 0x100:
//     - track 9: if the word at +8 is 0, store 3 there, call rand() and store (rand() % 8) + 1 at +4 (C remainder; the sign case is
//       handled as signed); with +8 nonzero, +4 is left alone.
//     - any other track except -1: store track at +4. Track -1 leaves +4 alone.
//     Then t = word at +4. If t is 0, call Stop__13VIBattleMusicb(music, 0); otherwise format t with "bgm\battle%02d.bgm" into a
//     256-byte stack buffer and call PlayBgm__13VISoundDeviceiPCc(device, 0, buffer). Return 0.
//  3. Otherwise: store 0 at +20. cur = word at +4. If track is -1: return -1 when cur is -1 (the only non-zero result); otherwise continue with track = cur.
//     If track != cur, call Preload__13VIBattleMusici(music, track) and store track at +4. Then, if the word at +8 is 0:
//     when the word at +28 is not -1 call StopNow__13VIBattleMusic(music) and reread +8 afterwards; store 0 at +12. Finally, if
//     the (current) +8 is not 3 store 1 there. Return 0.
// Inferred: +4 is the current track number (0 means none), +8 the state (0 idle, 1 start requested, 2 fading out, 3 playing), +12 the
// fade volume, +20 the "stop immediately" bool of Stop__13VIBattleMusicb, +28 a device playback id (-1 none). The feature-flag branch
// picks a random one of eight battle tracks for track 9; the file name pattern is the observed format string.
// Unknown: the meaning of the feature-flag bit, why track 9 is special, and the Preload/PlayBgm effects beyond their arguments.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned VIGetFeatureFlags__Fv();
int rand();
int sprintf(char* out, const char* format, ...);
void Stop__13VIBattleMusicb(void* music, bool immediate);
void StopNow__13VIBattleMusic(void* music);
void Preload__13VIBattleMusici(void* music, int track);
void PlayBgm__13VISoundDeviceiPCc(unsigned device, int index, const char* path);

int Play__13VIBattleMusici(void* music, int track) {
    unsigned* m = static_cast<unsigned*>(music);
    unsigned scene = m[0];
    if (!scene) return 0;
    unsigned device = words_at(scene)[19360 / 4];
    if (!device) return 0;
    if (VIGetFeatureFlags__Fv() & 0x100) {
        if (track == 9) {
            if (m[2] == 0) {
                m[2] = 3;
                m[1] = static_cast<unsigned>(rand() % 8 + 1);
            }
        } else if (track != -1) {
            m[1] = static_cast<unsigned>(track);
        }
        int current = static_cast<int>(m[1]);
        if (current == 0) {
            Stop__13VIBattleMusicb(music, false);
            return 0;
        }
        char path[256];
        sprintf(path, "bgm\\battle%02d.bgm", current);
        PlayBgm__13VISoundDeviceiPCc(device, 0, path);
        return 0;
    }
    m[5] = 0;
    int current = static_cast<int>(m[1]);
    if (track == -1) {
        if (current == -1) return -1;
        track = current;
    }
    if (track != current) {
        Preload__13VIBattleMusici(music, track);
        m[1] = static_cast<unsigned>(track);
    }
    if (m[2] == 0) {
        if (static_cast<int>(m[7]) != -1) StopNow__13VIBattleMusic(music);
        m[3] = 0;
    }
    if (m[2] != 3) m[2] = 1;
    return 0;
}
}
