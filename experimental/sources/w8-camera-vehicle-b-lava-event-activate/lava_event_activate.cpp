// Observed eventActivate__11LavaTractor at 0x284c08, extent 164, in order:
//   handle A = soundPlaySequence(lump "tractor.lmp" entry "lavatractor_idle.anm", object, 1.0), stored at object+2360
//   handle B = soundPlaySequence(same lump, "lavatractor_move.anm", object, 1.0), stored at object+2364
//   soundChangeSequenceVolume(B, 0.0)
//   soundMakePersistant(A), then soundMakePersistant(B), each reloaded from the object.
// The strings are at 0x476d50, 0x476d60 and 0x476d78. Handle A is stored before
// the second lump lookup.
// Unknown: the meaning of persistence in the sound layer.
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
extern "C" void soundChangeSequenceVolume__Fif(int handle, float volume);
extern "C" void soundMakePersistant__Fi(int handle);

namespace {
typedef int __attribute__((may_alias)) AliasedInt;
char tractor_lump[] = "tractor.lmp";
char idle_name[] = "lavatractor_idle.anm";
char move_name[] = "lavatractor_move.anm";
}

extern "C" void eventActivate__11LavaTractor(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    *reinterpret_cast<AliasedInt*>(o + 2360) = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(tractor_lump, idle_name), object, 1.0f);
    const int move = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(tractor_lump, move_name), object, 1.0f);
    *reinterpret_cast<AliasedInt*>(o + 2364) = move;
    soundChangeSequenceVolume__Fif(move, 0.0f);
    soundMakePersistant__Fi(*reinterpret_cast<AliasedInt*>(o + 2360));
    soundMakePersistant__Fi(*reinterpret_cast<AliasedInt*>(o + 2364));
}
