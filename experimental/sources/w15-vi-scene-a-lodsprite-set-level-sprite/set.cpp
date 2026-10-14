// Observed: SetLevelSprite__11VILODSpriteiiR7VIScene at 0x01076840, 124 bytes. Arguments: the LOD sprite (L), a level number (N), a sprite
// index (X) and the scene (S). The entry is the 32-bit word at L + 76 + 8*N. If it is not -1 the function calls
// ReleaseSprite__7VIScenei(S, entry) and sets the entry to -1 (the result is not used). It then stores X in the entry, calls
// ShareSprite__7VIScenei(S, X) and returns 0. N is not range checked; the second word of each 8-byte entry is not touched.
// Inferred: a table of per-level sprite indices that replaces its occupant, releasing the old one and taking a share of the new one.
// Unknown: the table's length and what the other word of each entry holds.
extern "C" {
int ReleaseSprite__7VIScenei(unsigned char* scene, int index);
int ShareSprite__7VIScenei(unsigned char* scene, int index);

int SetLevelSprite__11VILODSpriteiiR7VIScene(unsigned char* self, int level, int sprite, unsigned char* scene) {
    int* entry = reinterpret_cast<int*>(self + 76 + level * 8);
    if (*entry != -1) {
        ReleaseSprite__7VIScenei(scene, *entry);
        *entry = -1;
    }
    *entry = sprite;
    ShareSprite__7VIScenei(scene, sprite);
    return 0;
}
}
