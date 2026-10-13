// Observed converttobcd at 0x002d13e8, 104 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to an eight-byte clock record. Six of its bytes are converted in place, binary to packed decimal,
// by the local routine tobcd (0x002d1330), one call per byte, in the order offsets 7, 6, 5, 3, 2, 1.
// Offsets 0 and 4 are not read or written. Nothing is returned.
// Inferred: the record is the console clock (offset 1 seconds, 2 minutes, 3 hours, 5 day, 6 month, 7 year in
// AdjustTime's use of offsets 2 and 3); the field names beyond those two are not established here.
// Unknown: what bytes 0 and 4 hold.
// Callers: AdjustTime and the clock readers of the same library. tobcd is not authored in this unit.
extern "C" {
unsigned char tobcd(unsigned char value);

void converttobcd(unsigned char* clock) {
    clock[7] = tobcd(clock[7]);
    clock[6] = tobcd(clock[6]);
    clock[5] = tobcd(clock[5]);
    clock[3] = tobcd(clock[3]);
    clock[2] = tobcd(clock[2]);
    clock[1] = tobcd(clock[1]);
}
}
