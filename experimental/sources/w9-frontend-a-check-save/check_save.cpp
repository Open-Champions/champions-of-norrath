// Observed: checkSave__FP10GameObject at 0x001cdb38 is 284 bytes (a file-local function in the
// image). The argument is a GameObject whose vtable pointer is at byte 72. A null object calls
// __throw_bad_typeid. Otherwise slot 1 of the vtable (byte 4) is called with the object and
// its result compared with __tf7Trigger() through __eq__C9type_infoRC9type_info; anything but 1
// returns. For a Trigger the string pointer at byte 92 must be nonzero and strcmp-equal to
// "noSaveZone", else it returns. Then the word saveOk is loaded, and for each of the first
// nmPlayers records of player (stride 23984; the count is read once) whose pointer at byte 21872
// is nonzero and whose float at byte 172 is greater than 0.0f: when the object's floats at
// bytes 76, 80, 84, 88 form a box (x low, y low, x high, y high) that contains the point formed by
// the floats at bytes 16 and 20 behind that pointer (all comparisons inclusive), the loaded value
// becomes 0. The result is stored back to saveOk after the loop. Nothing is returned.
// Inferred: the vtable slot called is the compiler's type-information accessor, because its result
// is compared with Trigger's type_info; the box is the no-save zone of the trigger.
// Unknown: what the float at record byte 172 and the box layout names are.

extern "C" {
extern int saveOk;
extern int nmPlayers;
extern unsigned char player[];
void __throw_bad_typeid();
void* __tf7Trigger();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
int strcmp(const char* left, const char* right);
#if defined(NORRATH_HOST_CONTRACT)
void** norrath_host_vtable(void* object);
#endif
}

namespace {
void** vtable_of(unsigned char* bytes) {
#if defined(NORRATH_HOST_CONTRACT)
    // A host pointer is wider than the 4-byte slot at byte 72 and would overlap the float at 76.
    return norrath_host_vtable(bytes);
#else
    return *reinterpret_cast<void***>(bytes + 72);
#endif
}
}

extern "C" void checkSave__FP10GameObject(void* object) {
    unsigned char* const bytes = static_cast<unsigned char*>(object);
    void* type = 0;
    if (object != 0) {
        void** const vtable = vtable_of(bytes);
        type = reinterpret_cast<void* (*)(void*)>(vtable[1])(object);
    } else {
        __throw_bad_typeid();
    }
    if (__eq__C9type_infoRC9type_info(type, __tf7Trigger()) != 1) {
        return;
    }
    const char* const zone = *reinterpret_cast<const char**>(bytes + 92);
    if (zone == 0) {
        return;
    }
    if (strcmp(zone, "noSaveZone") != 0) {
        return;
    }
    int count = nmPlayers;
    if (count <= 0) {
        return;
    }
    int ok = saveOk;
    const float low_x = *reinterpret_cast<float*>(bytes + 76);
    const float low_y = *reinterpret_cast<float*>(bytes + 80);
    const float high_x = *reinterpret_cast<float*>(bytes + 84);
    const float high_y = *reinterpret_cast<float*>(bytes + 88);
    for (int i = 0; i < count; ++i) {
        unsigned char* const record = player + i * 23984;
        unsigned char* const other = *reinterpret_cast<unsigned char**>(record + 21872);
        if (other == 0) {
            continue;
        }
        if (!(0.0f < *reinterpret_cast<float*>(record + 172))) {
            continue;
        }
        const float x = *reinterpret_cast<float*>(other + 16);
        const float y = *reinterpret_cast<float*>(other + 20);
        if (low_x <= x && low_y <= y && x <= high_x && y <= high_y) {
            ok = 0;
        }
    }
    saveOk = ok;
}
