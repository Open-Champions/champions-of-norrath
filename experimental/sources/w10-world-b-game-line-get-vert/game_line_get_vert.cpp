// Observed gameLineGetVert__Fii at 0x00279bc8, 44 bytes in SLUS_205.65. Arguments: a line
// number and a point index, both signed. The line number selects a record pointer from the
// 100-pointer table gameLines (0x008d57a8). The record holds a count of points at +4 and a
// pointer to the points at +20; each point is 64 bytes. The point index is reduced with a
// signed remainder by the count (so it wraps around, and a negative index gives a negative
// remainder), and the address of that point is returned. Nothing is written.
// Inferred: the 64-byte stride is the size of one path vertex; only its first 12 bytes (a
// position) are read by gameLineGetNextPoint. Unknown: the count's lower bound (a zero count
// divides by zero in the image, which has no check) and the rest of the record and vertex.
extern unsigned char* gameLines[100];

extern "C" unsigned char* gameLineGetVert__Fii(int line, int index) {
    unsigned char* record = gameLines[line];
    const int count = *reinterpret_cast<int*>(record + 4);
    unsigned char* points = *reinterpret_cast<unsigned char**>(record + 20);
    return points + (index % count) * 64;
}
