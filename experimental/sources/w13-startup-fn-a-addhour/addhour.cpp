// Observed addhour at 0x002d15b0, 48 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to the eight-byte clock record used by AdjustTime. The byte at offset 3 is
// incremented and stored back. When the stored value is 24 (compared as an unsigned byte), it is
// reset to 0 and the routine finishes by a tail call to adddate (0x002d1450) with the same record, so
// whatever adddate does is the end of the operation. Nothing is returned.
// Inferred: offset 3 is the hour in binary, the record having been through convertfrombcd, and adddate
// advances the date fields at offsets 5, 6 and 7.
// Unknown: behaviour when the hour byte is already 24 or more (it is incremented without limit and wraps at 256).
// Callers: AdjustTime. adddate is not authored in this unit.
extern "C" {
void adddate(unsigned char* clock);

void addhour(unsigned char* clock) {
    unsigned char hour = clock[3] + 1;
    clock[3] = hour;
    if (hour == 24) {
        clock[3] = 0;
        adddate(clock);
    }
}
}
