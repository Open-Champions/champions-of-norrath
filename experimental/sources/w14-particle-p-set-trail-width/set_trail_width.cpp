// Observed: P_SetTrailWidth__Fii at 0x00182a80 is 64 bytes. A zero first argument returns
// without effect. Otherwise the record is trail + 1040 * (id & 0xff); the signed halfword
// at record offset 1024 is compared with the whole id, and on equality the byte at record
// offset 1039 is set to the arithmetic shift right by two of the second argument. The
// record index is not range-checked against the 64 records of trail.
// Inferred: id is a trail handle whose low byte selects the record and whose full value is
// checked against the record's sequence stamp, so a stale handle does nothing; the second
// argument is a width in quarter units. Names come from the symbol only.
// Unknown: the unit of the width, what draws the byte, and where the stamp is written.
extern "C" {
extern unsigned char trail[64 * 1040];

void P_SetTrailWidth__Fii(int id, int width) {
    if (id != 0) {
        unsigned char* record = trail + (id & 0xff) * 1040;
        if (*reinterpret_cast<short*>(record + 1024) == id) {
            record[1039] = static_cast<unsigned char>(width >> 2);
        }
    }
}
}
