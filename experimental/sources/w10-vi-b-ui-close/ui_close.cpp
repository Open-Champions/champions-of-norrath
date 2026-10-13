// Observed Close__4VIUI at 0x0102f288, 64 bytes. It calls
// Unload__13VIStringTable on the three string tables at offsets 0, 16 and 32
// of the object, in that order, then clears the words at offsets 72, 80 and
// 76 (written in that order). It returns nothing. Offsets 72, 76 and 80 are
// the ones Init__4VIUI fills with its three arguments.
extern "C" {
struct VIUI;
struct VIStringTable;
void Unload__13VIStringTable(VIStringTable* table);

void Close__4VIUI(VIUI* ui) {
    char* base = reinterpret_cast<char*>(ui);
    Unload__13VIStringTable(reinterpret_cast<VIStringTable*>(base));
    Unload__13VIStringTable(reinterpret_cast<VIStringTable*>(base + 16));
    Unload__13VIStringTable(reinterpret_cast<VIStringTable*>(base + 32));
    *reinterpret_cast<unsigned*>(base + 72) = 0;
    *reinterpret_cast<unsigned*>(base + 80) = 0;
    *reinterpret_cast<unsigned*>(base + 76) = 0;
}
}
