// Observed findCommander__14WoodElfSoldier at 0x00249408, extent 232. If the word
// at object+960 is nonzero it first calls leaveCommander__14WoodElfSoldier(object).
// It then scans the ten words of the static table _14WoodElfSoldier$m_commanderList
// (0x0042dd90, 40 bytes) in order. A zero entry is skipped. Otherwise word 3 (+12)
// of the entry points at a candidate soldier. A candidate whose word at +12 has
// bit 3 (value 8) set, or whose word at +960 is nonzero, is skipped. The squared
// distance between the positions at +16 of the object and the candidate (x, then
// y, then z, summed in that order) is compared with the best so far, which starts
// at the float word at 0x00482004 (observed 360000.0); a strictly smaller value
// becomes the new best. Returns the nearest candidate, or zero.
// Inferred: the table lists soldiers that currently lead a group, and 360000 would
// be a squared radius of 600. Unknown: what bit 3 of the +12 word and the +960
// word mean. The 3Orc counterpart has the same shape without the +960 test.
struct WoodElfSoldier;
struct CommanderNode {
    unsigned char pad[12];
    WoodElfSoldier* soldier;
};
extern "C" {
void leaveCommander__14WoodElfSoldier(WoodElfSoldier* self);
extern CommanderNode* WoodElfSoldier_commanderList[10] __asm__("_14WoodElfSoldier$m_commanderList");
WoodElfSoldier* findCommander__14WoodElfSoldier(WoodElfSoldier* self) {
    unsigned char* o = reinterpret_cast<unsigned char*>(self);
    float best = 360000.0f;
    if (*reinterpret_cast<int*>(o + 960)) leaveCommander__14WoodElfSoldier(self);
    WoodElfSoldier* found = 0;
    for (int i = 0; i < 10; ++i) {
        const CommanderNode* node = WoodElfSoldier_commanderList[i];
        if (!node) continue;
        unsigned char* c = reinterpret_cast<unsigned char*>(node->soldier);
        if (*reinterpret_cast<unsigned*>(c + 12) & 8u) continue;
        if (*reinterpret_cast<int*>(c + 960)) continue;
        const float* mine = reinterpret_cast<const float*>(o + 16);
        const float* theirs = reinterpret_cast<const float*>(c + 16);
        const float dx = mine[0] - theirs[0];
        const float dy = mine[1] - theirs[1];
        const float dz = mine[2] - theirs[2];
        const float d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            found = node->soldier;
            best = d;
        }
    }
    return found;
}
}
