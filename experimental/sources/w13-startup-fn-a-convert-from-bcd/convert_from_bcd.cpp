// Observed convertfrombcd at 0x002d1380, 104 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to an eight-byte clock record. Six of its bytes are converted in place, packed decimal to binary,
// by the local routine frombcd (0x002d1360), one call per byte, in the order offsets 7, 6, 5, 3, 2, 1.
// Offsets 0 and 4 are not read or written. Nothing is returned.
// Inferred: the record is the console clock (offset 1 seconds, 2 minutes, 3 hours, 5 day, 6 month, 7 year in
// AdjustTime's use of offsets 2 and 3); the field names beyond those two are not established here.
// Unknown: what bytes 0 and 4 hold.
// Callers: AdjustTime and the clock readers of the same library. frombcd is not authored in this unit.
extern "C" {
unsigned char frombcd(unsigned char value);

void convertfrombcd(unsigned char* clock) {
    clock[7] = frombcd(clock[7]);
    clock[6] = frombcd(clock[6]);
    clock[5] = frombcd(clock[5]);
    clock[3] = frombcd(clock[3]);
    clock[2] = frombcd(clock[2]);
    clock[1] = frombcd(clock[1]);
}
}
