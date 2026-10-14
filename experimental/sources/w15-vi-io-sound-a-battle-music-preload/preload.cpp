// Observed: Preload__13VIBattleMusici at 0x0106dc48, 236 bytes. Arguments: the battle-music object (this) and a track number N.
//  - If VIGetFeatureFlags__Fv() has bit 0x100 set, or the scene pointer at +0 is null, or the word at scene + 19360 (S) is zero,
//    the result is 0 and nothing else happens.
//  - Otherwise it formats the path "bgm\battle%d.esf" with N into a 256-byte stack buffer, calls Unload__13VIBattleMusic(this), and
//    if N is above zero: constructs a VIESFParse in a 1072-byte stack area, calls ParseSound__10VIESFParsePCcR7VIScene(parse, path,
//    scene), stores the result at +24 whether or not it is negative, and when it is not negative fetches
//    Sound__13VISoundDevicei(S, result) and calls SetRepeat__4VIXmi(sound, 1); then the parse object is destroyed with
//    _$_10VIESFParse(parse, 2).
//  - The word at +4 is set to N.
//  - The result is 0 when N is zero, otherwise -1 if the signed word at +24 is negative and 0 if not. (For N below zero the parse
//    is skipped, so +24 is whatever Unload left.)
// Inferred: S is the scene's sound device; +4 is the loaded track number and +24 the sound handle (Unload releases it and sets it
// back to -1); the result is -1 when the track could not be loaded. The file is parsed as a sound resource and told to repeat.
// Unknown: the member names, the layout of VIScene beyond +19360 and +19352 (used by Unload), and why the parse object is
// 1072 bytes (taken from the stack space the image reserves; the constructor writes up to offset 1064).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
unsigned VIGetFeatureFlags__Fv();
int sprintf(char* destination, const char* format, ...);
void Unload__13VIBattleMusic(unsigned char* music);
void __10VIESFParse(void* parse);
int ParseSound__10VIESFParsePCcR7VIScene(void* parse, const char* name, unsigned char* scene);
void _$_10VIESFParse(void* parse, int flags);
unsigned Sound__13VISoundDevicei(unsigned char* device, int handle);
void SetRepeat__4VIXmi(unsigned sound, int repeat);

int Preload__13VIBattleMusici(unsigned char* music, int track) {
    if ((VIGetFeatureFlags__Fv() & 0x100) != 0) return 0;
    unsigned scene = word_at(music, 0);
    if (scene == 0) return 0;
    unsigned device = word_at(bytes_at(scene), 19360);
    if (device == 0) return 0;
    char path[256];
    sprintf(path, "bgm\\battle%d.esf", track);
    Unload__13VIBattleMusic(music);
    if (track > 0) {
        alignas(8) unsigned char parse[1072];
        __10VIESFParse(parse);
        int sound = ParseSound__10VIESFParsePCcR7VIScene(parse, path, bytes_at(word_at(music, 0)));
        *reinterpret_cast<int*>(music + 24) = sound;
        if (sound >= 0) SetRepeat__4VIXmi(Sound__13VISoundDevicei(bytes_at(device), sound), 1);
        _$_10VIESFParse(parse, 2);
    }
    *reinterpret_cast<int*>(music + 4) = track;
    if (track == 0) return 0;
    return *reinterpret_cast<int*>(music + 24) < 0 ? -1 : 0;
}
}
