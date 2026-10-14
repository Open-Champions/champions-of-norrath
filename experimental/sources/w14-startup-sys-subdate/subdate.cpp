// Observed subdate at 0x002d1508, 164 bytes, a local routine of the system-configuration (Scf) library.
// Argument: a pointer to the eight-byte clock record of AdjustTime, already converted to binary. It moves the
// date back by one day using the bytes at offset 5 (day, starting at 1), 6 (month, starting at 1) and 7 (year,
// 0..99), the mirror of adddate. The twelve month lengths (31 28 31 30 31 30 31 31 30 31 30 31, from 0x0047b158)
// are copied to the stack and February becomes 29 when the year read at entry is a multiple of four.
// The day is decremented and stored first. When it reaches 0 the month is decremented; a month that reaches 0
// becomes 12 and the year is decremented, with 0 wrapping to 99. The day is then set to the length of the
// resulting month (indexed as month - 1, no range check). Nothing is returned.
// Callers: subhour, and AdjustTime directly.
// Unknown: behaviour for a month byte outside 1..12 (the original reads outside its copy of the table).
extern "C" {
void subdate(unsigned char* clock) {
    unsigned char days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned char year = clock[7];
    unsigned char day = clock[5] - 1;
    clock[5] = day;
    if ((year & 3) == 0) days[1] = 29;
    if (clock[5] == 0) {
        unsigned char month = clock[6] - 1;
        clock[6] = month;
        if (month == 0) {
            if (clock[7] == 0) clock[7] = 99;
            else clock[7] = clock[7] - 1;
            clock[6] = 12;
        }
        clock[5] = days[clock[6] - 1];
    }
}
}
