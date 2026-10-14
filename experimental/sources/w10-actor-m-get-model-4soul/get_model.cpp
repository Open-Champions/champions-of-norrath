#include "../w10-actor-m-common/access.hpp"
// Observed getModel__4Soul at 0x1f5010, 32 bytes. The word at object+36 is the address of a table of
// resource pointers. When the word at object+960 is nonzero the table entry at +28 is returned,
// otherwise the entry at +12. Inferred: both entries are model handles, from the routine's name.
// Unknown: what the word at +960 holds (the constructor stores a minion list head there).
extern "C" unsigned getModel__4Soul(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* table = w10m::from_word(w10m::word(o, 36));
    return w10m::word(o, 960) != 0 ? w10m::word(table, 28) : w10m::word(table, 12);
}
