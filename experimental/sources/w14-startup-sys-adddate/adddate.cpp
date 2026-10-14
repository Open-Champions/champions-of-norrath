// Observed adddate at 0x002d1450, 180 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to the eight-byte clock record of AdjustTime, already converted to binary. It advances the
// date by one day using the bytes at offset 5 (day, starting at 1), 6 (month, starting at 1) and 7 (year, 0..99).
// The month lengths come from a twelve-byte table read at 0x0047b158 (31 28 31 30 31 30 31 31 30 31 30 31),
// copied to the stack because February is changed to 29 when the year is a multiple of four (year 0 counts).
// The day is incremented and stored first. When it exceeds the length of the month named by the month byte
// (indexed as month - 1, no range check), the day becomes 1 and the month is incremented; a month that reaches
// 13 becomes 1 and the year is incremented, with 99 wrapping to 0. Nothing is returned.
// Callers: addhour, and AdjustTime directly. Nothing else of the record is touched.
// Unknown: behaviour for a month byte of 0 or above 12 beyond what the indexing above implies (the original
// reads outside its copy of the table).
extern "C" {
void adddate(unsigned char* clock) {
    unsigned char days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned char year = clock[7];
    unsigned char day = clock[5] + 1;
    clock[5] = day;
    if ((year & 3) == 0) days[1] = 29;
    if (days[clock[6] - 1] < clock[5]) {
        clock[5] = 1;
        unsigned char month = clock[6] + 1;
        clock[6] = month;
        if (month == 13) {
            if (clock[7] == 99) clock[7] = 0;
            else clock[7] = clock[7] + 1;
            clock[6] = 1;
        }
    }
}
}
