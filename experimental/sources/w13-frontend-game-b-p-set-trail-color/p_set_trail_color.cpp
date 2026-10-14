// Observed P_SetTrailColor__Fiiiii at 0x001829d8, extent 72. (id, a, b, c, d). For
// a nonzero id, the trail record selected by the low byte of the id (trail at
// 0x00780b00, stride 1040) is checked: if its halfword at +1024 equals the whole
// id (as a signed halfword compared with the int), the low bytes of the
// arguments are stored: a at +1034, b at +1035, c at +1036, d at +1037.
// Any other case changes nothing. Returns nothing.
// Inferred: a, b, c, d are red, green, blue and alpha in that storage order;
// the argument order of the fifth is not established by a caller.
extern "C" {
extern unsigned char trail[];
void P_SetTrailColor__Fiiiii(int id, int a, int b, int c, int d) {
    if (id != 0) {
        unsigned char* record = trail + (static_cast<unsigned>(id) & 0xffu) * 1040u;
        if (*reinterpret_cast<short*>(record + 1024) == id) {
            record[1037] = static_cast<unsigned char>(d);
            record[1034] = static_cast<unsigned char>(a);
            record[1035] = static_cast<unsigned char>(b);
            record[1036] = static_cast<unsigned char>(c);
        }
    }
}
}
