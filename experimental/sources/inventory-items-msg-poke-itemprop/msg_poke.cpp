// Observed msg_poke__8ItemPropi at 0x1559c8, 236 bytes.
// The item description is at object+328. When bit 39 of the 64-bit value at
// object+336 (bit 7 of the byte at object+340) is set, playerMaybeGetItem runs
// for each player number from 0 up to nmPlayers, re-reading nmPlayers every
// time, and the object counts as taken if any call returned non-zero. When the
// bit is clear, only playerMaybeGetItem(argument, object+328) decides. A taken
// object calls through its own vtable: the word at object+72 plus 8 locates a
// slot with a signed halfword adjustment at +0 and a function address at +4,
// and the function receives object+adjustment and 3. The call is skipped when
// the object pointer is null. Otherwise nothing happens.
// Unknown: what the flag bit means; the vtable slot at +8 is the deleting
// destructor form for the ItemProp vtable, which is inferred from the flag 3.
// The host check defines NORRATH_INVENTORY_HOST and supplies host_slot and
// host_call because host addresses do not fit in 32-bit vtable words.

extern "C" {
extern int nmPlayers;
int playerMaybeGetItem__FiPQ24item11description(int player, void* description);
}

#if defined(NORRATH_INVENTORY_HOST)
extern "C" const unsigned char* host_slot(unsigned vtable_word);
extern "C" void host_call(unsigned function, void* self, int flag);
#endif

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void call_slot(unsigned char* object) {
#if defined(NORRATH_INVENTORY_HOST)
    const unsigned char* slot = host_slot(read_word(object + 72)) + 8u;
#else
    const unsigned char* slot = reinterpret_cast<const unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(read_word(object + 72)) + 8u);
#endif
    const int adjustment = static_cast<short>(unsigned(slot[0]) | (unsigned(slot[1]) << 8));
    const unsigned function = read_word(slot + 4);
#if defined(NORRATH_INVENTORY_HOST)
    host_call(function, object + adjustment, 3);
#else
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjustment, 3);
#endif
}
}

extern "C" void msg_poke__8ItemPropi(void* object_storage, int player_number) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    if ((object[340] & 0x80u) != 0u) {
        bool taken = false;
        for (int player = 0; player < nmPlayers; ++player) {
            if (playerMaybeGetItem__FiPQ24item11description(player, object + 328) != 0) taken = true;
        }
        if (!taken) return;
    } else if (playerMaybeGetItem__FiPQ24item11description(player_number, object + 328) == 0) {
        return;
    }
    if (object != nullptr) call_slot(object);
}
