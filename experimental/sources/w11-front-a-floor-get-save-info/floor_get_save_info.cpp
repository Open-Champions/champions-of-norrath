// Observed: floorGetSaveInfo__FPci at 0x00191bd8 is 520 bytes. Arguments: destination and limit;
// the result is the number of bytes written, counting an 8-byte header. sprGrid (0x007ac1f0) is
// a 30 x 30 array of pointers, 120 bytes per row. A cell pointer is zero or points to an object
// that holds 24-byte records from its start (31 of them) and a signed count word at +760.
//  1. Output starts at destination + 8, the record count at 0.
//  2. A limit below 21 writes nothing more: the word at destination is 0 and 8 is returned.
//  3. Otherwise z runs over 0..30, row over 0..29 and column over 0..29 (z outermost, column
//     innermost); for a nonzero cell whose count word is greater than z, with r = cell + 24 * z,
//     a 12-byte record is appended and the record count rises by one:
//       +0  halfword  the halfword at r + 8
//       +2  halfword  the halfword at r + 10
//       +4  halfword  the halfword at r + 12
//       +6  halfword  (int)(sqrtf((float)(X * X + Y * Y)) + 0.5f), X the signed halfword at
//                     r + 18 and Y the signed halfword at r + 16 (the sum is an int sum)
//       +8  halfword  iatan2__Fff((float)Y, (float)X), low 16 bits
//       +10 byte      (the signed halfword at r + 20 - 979) >> 1 (arithmetic), low 8 bits
//       +11 byte      the original copies a stale stack byte here; zero is written instead.
//     After each record the walk stops when the bytes written so far (the output end minus
//     destination, header included) exceed limit - 256.
//  4. The record count is stored at destination as an int; the bytes written are returned.
// Inferred: the records describe objects in the floor grid (a position-like pair, a distance and
// a bearing from the cell origin), saved for the level image; the cell objects are the floor
// sprite packets.
// Unknown: what r + 8, +10, +12 and +20 hold, why 979 is subtracted, and the cell object type.
extern "C" {
extern unsigned char* sprGrid[30][30];
int iatan2__Fff(float y, float x);
}

extern "C" int floorGetSaveInfo__FPci(char* destination, int limit) {
    unsigned char* out = reinterpret_cast<unsigned char*>(destination) + 8;
    int count = 0;
    if (limit >= 21) {
        for (int z = 0; z < 31; ++z) {
            for (int row = 0; row < 30; ++row) {
                for (int col = 0; col < 30; ++col) {
                    unsigned char* cell = sprGrid[row][col];
                    if (cell == nullptr || !(z < *reinterpret_cast<int*>(cell + 760))) continue;
                    unsigned char* r = cell + 24 * z;
                    ++count;
                    unsigned short record[6];
                    record[0] = *reinterpret_cast<unsigned short*>(r + 8);
                    record[1] = *reinterpret_cast<unsigned short*>(r + 10);
                    record[2] = *reinterpret_cast<unsigned short*>(r + 12);
                    const int y = *reinterpret_cast<short*>(r + 16);
                    const int x = *reinterpret_cast<short*>(r + 18);
                    record[3] = static_cast<unsigned short>(static_cast<int>(__builtin_sqrtf(static_cast<float>(x * x + y * y)) + 0.5f));
                    record[4] = static_cast<unsigned short>(iatan2__Fff(static_cast<float>(y), static_cast<float>(x)));
                    const int tilt = (*reinterpret_cast<short*>(r + 20) - 979) >> 1;
                    record[5] = static_cast<unsigned short>(static_cast<unsigned char>(tilt));
                    __builtin_memcpy(out, record, 12);
                    out += 12;
                    if (limit - 256 < static_cast<int>(out - reinterpret_cast<unsigned char*>(destination))) goto done;
                }
            }
        }
    }
done:
    *reinterpret_cast<int*>(destination) = count;
    return static_cast<int>(out - reinterpret_cast<unsigned char*>(destination));
}
