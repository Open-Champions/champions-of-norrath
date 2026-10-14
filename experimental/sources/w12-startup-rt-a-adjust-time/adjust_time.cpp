// Observed AdjustTime at 0x002d1608, 140 bytes, a local routine of the system-configuration (Scf) library.
// Arguments: a pointer to an eight-byte clock record whose fields are packed decimal, and a signed
// number of minutes. The record is converted to binary in place (convertfrombcd), the minutes are
// added to the byte at offset 2, and the result is carried into the hour: while it is below zero, 60 is
// added and subhour runs; while it is 60 or more, 60 is taken off and addhour runs. The remaining
// value is stored back at offset 2 and the record is converted to packed decimal again by a tail call to
// converttobcd, so whatever that returns is the result. Nothing else in the record is read here.
// Inferred: offset 2 holds minutes and offset 3 hours (addhour/subhour move offset 3 and carry into the
// date). The four helpers are local routines of the same library and are not authored in this unit.
// Callers: sceScfGetLocalTimefromRTC passes the zone and summer-time offset.
extern "C" {
void convertfrombcd(unsigned char* clock);
void converttobcd(unsigned char* clock);
void addhour(unsigned char* clock);
void subhour(unsigned char* clock);

void AdjustTime(unsigned char* clock, int minutes) {
    convertfrombcd(clock);
    int total = clock[2] + minutes;
    if (total < 0) {
        do {
            total += 60;
            subhour(clock);
        } while (total < 0);
    } else {
        while (total >= 60) {
            addhour(clock);
            total -= 60;
        }
    }
    clock[2] = static_cast<unsigned char>(total);
    converttobcd(clock);
}
}
