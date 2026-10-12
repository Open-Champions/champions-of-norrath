#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_poke__5Leveri at 0x21ca50, 132 bytes.
// Clears bit 20 of the word at object+12 (mask 0xffefffff). eRand on the
// object picks 1 or 2 from its low bit, and that number formats the name
// "game_leverpull%d.anm". The name is looked up in the "fx.lmp" lump, the
// sequence found is started on the object at rate 1.0, the object is added to
// the run list, and the byte at object+81 is set to 1. The integer argument
// is unused. Unknown: what bit 20 of the flag word and the byte at +81 mean.
extern "C" int eRand__10GameObject(void* object);
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" void msg_poke__5Leveri(void* self, int value) {
    using namespace vslots_b;
    (void)value;
    write_word(address(self) + 12u, read_word(address(self) + 12u) & 0xffefffffu);
    char name[32];
    static char format[] = "game_leverpull%d.anm";
    sprintf(name, format, (eRand__10GameObject(self) & 1) + 1);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), self, 1.0f);
    norrath_actor_add_run_list(self);
    write_u8(address(self) + 81u, 1u);
}
