// Observed amxSoundSequence__FP5__amxPi at 0x0013b180, 156 bytes. Cells 1
// and 2 are AMX strings. The resource is played as a sequence at no object
// with float 1, and that call's result is passed to
// soundMakeSequenceUnpausable. Both calls happen when the resource is null.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
extern "C" void soundMakeSequenceUnpausable__Fi(int sequence);

extern "C" int amxSoundSequence__FP5__amxPi(void* amx, int* params) {
    char group[160];
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(group, physical);
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(name, physical);
    void* header = lumpFindResource__FPcT0(group, name);
    soundMakeSequenceUnpausable__Fi(soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, nullptr, 1.f));
    return 0;
}
