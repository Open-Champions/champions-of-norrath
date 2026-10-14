// Observed: MC_HasThisLevelBeenSaved__Fv at 0x0025b308 is 136 bytes. It builds the
// full level name in a 256-byte stack buffer with gameGetFullLevelName__FPc. It
// then scans 24-byte records starting at byte 112144 of ramSlot, at most 26 of
// them (624 bytes). The scan stops at the first record whose first byte is zero.
// A record whose bytes compare equal to the name through strcmp returns 1;
// otherwise the result is 0.
// Inferred: nothing.
// Unknown: the layout of the rest of each 24-byte record.

extern "C" void gameGetFullLevelName__FPc(char* name);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char ramSlot[];

extern "C" int MC_HasThisLevelBeenSaved__Fv() {
    char name[256];
    gameGetFullLevelName__FPc(name);
    char* record = ramSlot + 112144;
    char* end = record + 624;
    if (*record != 0) {
        do {
            if (strcmp(record, name) == 0) {
                return 1;
            }
            record += 24;
        } while (record < end && *record != 0);
    }
    return 0;
}
