// Observed showHiddenObjects__FP10GameObject at 0x0018e308, 64 bytes. When bit 5 (0x20) of
// the object's flag word at +12 is set and bit 3 (0x08) of the byte at +570 is set, it
// calls objectUpdateInGrid__FP10GameObject on the object. It changes nothing itself and
// returns nothing.
// Inferred: the byte at +570 holds a hidden/removed state; the grid call re-registers the
// object. Unknown: the meaning of the two bits and what populates them.
struct GameObject;
extern "C" void objectUpdateInGrid__FP10GameObject(GameObject*);

extern "C" void showHiddenObjects__FP10GameObject(GameObject* object) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    if (!(*reinterpret_cast<int*>(bytes + 12) & 0x20)) return;
    if (!(bytes[570] & 0x08)) return;
    objectUpdateInGrid__FP10GameObject(object);
}
