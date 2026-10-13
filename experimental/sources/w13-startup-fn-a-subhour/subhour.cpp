// Observed subhour at 0x002d15e0, 36 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to the eight-byte clock record used by AdjustTime. When the byte at offset 3 is
// zero it becomes 23 and the routine finishes by a tail call to subdate (0x002d1508) with the same
// record; otherwise the byte is decremented and stored. Nothing is returned.
// Inferred: offset 3 is the hour in binary and subdate steps the date fields at offsets 5, 6 and 7 back.
// Unknown: nothing beyond the hour byte is examined here, so any value from 1 to 255 simply loses one.
// Callers: AdjustTime. subdate is not authored in this unit.
extern "C" {
void subdate(unsigned char* clock);

void subhour(unsigned char* clock) {
    unsigned char hour = clock[3];
    if (hour == 0) {
        clock[3] = 23;
        subdate(clock);
    } else {
        clock[3] = hour - 1;
    }
}
}
