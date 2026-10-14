#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed sleepMonsters__8creaturev at 0x113770, 32 bytes. It passes the address
// of the file-local sleepMonster__FP10GameObject (0x113c10, 128 bytes) to
// objectIterateRunLists, whose body is not defined here.
// Observed sleepMonster: it returns unless bit 0x20 of the word at object+12 is
// set and bit 0x40 is clear. It also returns when the byte at object+570 is
// nonzero. Otherwise bit 0x10 of that byte is set. When the signed halfword at
// object+572 is not 10, the virtual slot at byte offset 184 of the table at
// object+72 is called with the adjusted object and 0. The list head at
// object+576 is then destroyed and its head word is left as the call leaves it.
// Inferred: the list routine is the already defined norrath_list_destroy
// (listDestroy__FP8ListHead). Unknown: the meaning of the flag bits, of 570,
// of the value 10 at 572, and of slot 184.
extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(void*));
extern "C" void norrath_list_destroy(void* head);

namespace {
typedef void (*SlotFunction)(void*, int);

void sleep_monster(void* object) {
    using namespace w8c;
    const Address base = address(object);
    const unsigned flags = read_word(base + 12u);
    if ((flags & 0x20u) == 0u || (flags & 0x40u) != 0u) return;
    if (read_u8(base + 570u) != 0u) return;
    write_u8(base + 570u, 0x10u);
    if (read_i16(base + 572u) != 10) {
        void* adjusted;
        SlotFunction function = slot_function<SlotFunction>(object, 184u, &adjusted);
        function(adjusted, 0);
    }
    norrath_list_destroy(static_cast<unsigned char*>(object) + 576u);
}
}

extern "C" void sleepMonsters__8creaturev() {
    objectIterateRunLists__FPFP10GameObject_v(sleep_monster);
}
