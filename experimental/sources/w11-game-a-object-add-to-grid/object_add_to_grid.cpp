// Observed objectAddToGrid__FP10GameObject at 0x00143720, 200 bytes. Links an object into one of
// the 120 x 120 cell lists of the object grid (sibling of objectUpdateInGrid, which unlinks first).
// The object's floats at +16 and +20 (x, y) are truncated to int; gx = (x + 15360) >> 8 and
// gy = (y + 15360) >> 8 (arithmetic shifts). The list head is
//   offTheGrid           when bit 0x2 of the word at +12 is set, or gx or gy is outside 0..119;
//   ghostObjectGrid cell when bit 0x800 of the word at +12 is set (cells are 4-byte heads,
//                        index gy * 120 + gx);
//   objectGrid cell      otherwise.
// Then listOnewayLink__FP8ListHeadT0Pv(head, object + 8, object) runs; the object's own list node
// is at +8. Nothing is returned. The head offTheGrid is the one at 0x486890 (reached through the
// global pointer in the image); a second symbol of that name exists at 0x486848.
// Inferred: bit 0x2 keeps an object out of the grid and bit 0x800 marks ghost objects.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char offTheGrid[];
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
}

extern "C" void objectAddToGrid__FP10GameObject(void* object) {
    unsigned char* self = static_cast<unsigned char*>(object);
    int x = static_cast<int>(*reinterpret_cast<float*>(self + 16));
    int y = static_cast<int>(*reinterpret_cast<float*>(self + 20));
    unsigned flags = *reinterpret_cast<unsigned*>(self + 12);
    int cell_x = (x + 15360) >> 8;
    int cell_y = (y + 15360) >> 8;
    unsigned char* head;
    if ((flags & 2) != 0 || cell_x < 0 || cell_x >= 120 || cell_y < 0 || cell_y >= 120) {
        head = offTheGrid;
    } else if ((flags & 0x800) != 0) {
        head = ghostObjectGrid + (cell_y * 120 + cell_x) * 4;
    } else {
        head = objectGrid + (cell_y * 120 + cell_x) * 4;
    }
    listOnewayLink__FP8ListHeadT0Pv(head, self + 8, self);
}
