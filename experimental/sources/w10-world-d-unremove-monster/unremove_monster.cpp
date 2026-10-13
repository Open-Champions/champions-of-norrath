// Observed unremoveMonster__FP10GameObject at 0x0018e3d8, 80 bytes. If bit 5 (0x20) of the
// flag word at +12 is set and the signed byte at +570 equals 4, it clears bit 2 (0x04) of
// that byte, clears bits 15 and 19 (mask 0xfff77fff) of the flag word, and then calls
// objectUpdateInGrid__FP10GameObject on the object. Otherwise it does nothing. No result.
// Inferred: state 4 at +570 is a removed state and this undoes it. Unknown: what bits 15
// and 19 of the flag word mean; the byte is compared as a whole, not as a bit test.
struct GameObject;
extern "C" void objectUpdateInGrid__FP10GameObject(GameObject*);

extern "C" void unremoveMonster__FP10GameObject(GameObject* object) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    int flags = *reinterpret_cast<int*>(bytes + 12);
    if (!(flags & 0x20)) return;
    if (static_cast<signed char>(bytes[570]) != 4) return;
    bytes[570] = bytes[570] & 0xfb;
    *reinterpret_cast<int*>(bytes + 12) = flags & 0xfff77fff;
    objectUpdateInGrid__FP10GameObject(object);
}
