// Observed upcast__C16__user_type_infoRC9type_infoPvPPv at 0x002fd388, 120 bytes: the public upcast entry of
// the plain user type_info. Arguments: the type_info (this), the target type_info, an object pointer and a
// pointer to a word that receives an adjusted object pointer.
// It zeroes a three-word record on its stack and calls the virtual slot at vtable + 16 of this. The vtable
// pointer is the word at this + 4; a slot is a halfword this-adjustment at +0 and the function at +4. The call
// is made as fn(this + adjustment, 6, target, object, &record). When the slot returns nonzero the routine
// returns 0 and stores nothing. Otherwise it stores record word 0 through the fourth argument and returns
// whether record word 1 is at least 6 (1 or 0).
// The constant 6, and the meaning of record words 1 and 2, are only what the do_upcast slots authored beside
// this show (word 0 object, word 1 access path value, word 2 a constant); no enumerator names are claimed.
// Unknown: whether the third record word is ever read here (it is only zeroed).
extern "C" {
struct NorrathUpcastRecord {
    void* object;
    int access_path;
    int marker;
};

typedef int (*UpcastSlot)(const void* self, int access_path, const void* target, void* object, NorrathUpcastRecord* record);

int upcast__C16__user_type_infoRC9type_infoPvPPv(const void* self, const void* target, void* object, void** adjusted) {
    NorrathUpcastRecord record = {0, 0, 0};
    const unsigned char* const slot = *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(self) + 4) + 16;
    const int adjustment = *reinterpret_cast<const short*>(slot);
    const UpcastSlot fn = *reinterpret_cast<const UpcastSlot*>(slot + 4);
    if (fn(static_cast<const unsigned char*>(self) + adjustment, 6, target, object, &record)) return 0;
    *adjusted = record.object;
    return record.access_path >= 6;
}
}
