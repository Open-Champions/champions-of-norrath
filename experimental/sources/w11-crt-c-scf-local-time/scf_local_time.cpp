// Observed sceScfGetLocalTimefromRTC at 0x002d16a0, 72 bytes. It takes a
// pointer to a clock record, calls sceScfGetTimeZone() and then
// sceScfGetSummerTime(), and tail-calls AdjustTime(record, zone + 60 *
// summer - 540). The record is passed through unchanged; the routine reads
// and writes nothing itself and returns whatever AdjustTime returns.
// Inferred: the zone is in minutes relative to a base of 540 (nine hours),
// and the summer-time value counts hours; AdjustTime is the routine that
// shifts a BCD clock record by that many minutes (the caller in the sorted
// list patch passes eight BCD bytes).
extern "C" {
int sceScfGetTimeZone();
int sceScfGetSummerTime();
void AdjustTime(unsigned char* clock, int minutes);

void sceScfGetLocalTimefromRTC(unsigned char* clock) {
    int zone = sceScfGetTimeZone();
    int summer = sceScfGetSummerTime();
    AdjustTime(clock, zone + (summer * 60 - 540));
}
}
