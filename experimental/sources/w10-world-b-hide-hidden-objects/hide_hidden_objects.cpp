// Observed hideHiddenObjects__FP10GameObject at 0x0018e348, 64 bytes in SLUS_205.65. When the
// object's flag word (+12) has bit 0x20 and the byte at +570 has bit 0x08, the object is
// unlinked from the grid with objectRemoveFromGrid__FP10GameObject. Otherwise nothing happens.
// Inferred: the two bits mean "hidden"; their names are unknown.
extern "C" void objectRemoveFromGrid__FP10GameObject(unsigned char* object);

extern "C" void hideHiddenObjects__FP10GameObject(unsigned char* object) {
    if ((*reinterpret_cast<unsigned*>(object + 12) & 0x20) == 0) return;
    if ((object[570] & 0x08) == 0) return;
    objectRemoveFromGrid__FP10GameObject(object);
}
