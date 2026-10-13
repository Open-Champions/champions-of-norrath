// Observed jumpGateAddDestination__Fv at 0x002846f8, 164 bytes. Records the current level in the
// jump-gate destination list. It asks gameGetFullLevelName__FPc for the full level name (into a
// local buffer of at most 64 bytes, per the image's frame) and then walks the first
// jumpGateDestCount entries of jumpGateEntries (32-byte slots, signed count: zero or negative skips
// the walk). If strcmp finds an entry equal to the name, nothing more happens. Otherwise the name is
// copied with strcpy to slot number (signed) jumpGateDestCount and the count byte is incremented.
// In the image the increment is stored before the copy (it sits in the call's delay slot); the
// store order is not observable by the callees. There is no capacity check against the 15 slots.
// Unknown: what clears the list (see the jump-gate data notes) and what the slot format is beyond a
// NUL-terminated name. Nothing here ran on the EE.
extern "C" {
extern signed char jumpGateDestCount;
extern char jumpGateEntries[];
void gameGetFullLevelName__FPc(char* buffer);
int strcmp(const char* first, const char* second);
char* strcpy(char* destination, const char* source);
}

extern "C" void jumpGateAddDestination__Fv() {
    char name[64];
    gameGetFullLevelName__FPc(name);
    for (int i = 0; i < jumpGateDestCount; ++i) {
        if (strcmp(jumpGateEntries + i * 32, name) == 0) return;
    }
    signed char slot = jumpGateDestCount;
    jumpGateDestCount = static_cast<signed char>(jumpGateDestCount + 1);
    strcpy(jumpGateEntries + slot * 32, name);
}
