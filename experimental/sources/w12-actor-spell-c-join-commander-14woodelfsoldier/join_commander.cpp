// Observed joinCommander__14WoodElfSoldier at 0x00249db0, 92 bytes. The argument is the soldier (S).
// findCommander__14WoodElfSoldier(S) is called first. When it returns null, null is returned and nothing else
// happens. Otherwise, with C the commander it found, listOnewayLink__FP8ListHeadT0Pv(S + 960, C, C) and then
// listOnewayLink__FP8ListHeadT0Pv(C + 960, S, S) are called (the list head at +960 of each object is linked to
// the other object, which is passed as both the link and the data), and 1 is returned.
// Inferred: the head at +960 is a soldier/commander membership list (it is written symmetrically).
// Nothing here ran on the EE.
extern "C" {
void* findCommander__14WoodElfSoldier(void* self);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);

int joinCommander__14WoodElfSoldier(void* self) {
    unsigned char* const soldier = static_cast<unsigned char*>(self);
    unsigned char* const commander = static_cast<unsigned char*>(findCommander__14WoodElfSoldier(self));
    if (commander == nullptr) return 0;
    listOnewayLink__FP8ListHeadT0Pv(soldier + 960, commander, commander);
    listOnewayLink__FP8ListHeadT0Pv(commander + 960, soldier, soldier);
    return 1;
}
}
