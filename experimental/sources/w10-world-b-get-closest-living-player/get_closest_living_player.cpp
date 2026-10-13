// Observed getClosestLivingPlayer__FG6Point3f at 0x0010eb18, 220 bytes in SLUS_205.65.
// Arguments: a position (a 12-byte Point3 passed by value, so by address) and a float limit.
// Walks the player records 0 .. nmPlayers-1 (0x00484538); a record is 23984 bytes and holds
// the player's object pointer at +21872, in the table `player` (0x00408600). A null object,
// or one with bit 0x08 set in its flag word (+12), is skipped. For the others the distance
// from the position to the object's position (three floats at +16) is the square root of
// the sum of squares, summed x, then y, then z. A player replaces the best when its distance
// is strictly less than the current limit (initially the argument), and the limit becomes
// that distance. Returns the index of the best player as a signed byte, or -1 when none was
// closer than the limit (or nmPlayers is not positive).
// Inferred: flag bit 0x08 means the object is dead or gone, from the function's name; not
// confirmed. Unknown: other uses of the position argument.
extern "C" unsigned char player[];
extern int nmPlayers;

struct Point3Words { float x, y, z; };

extern "C" signed char getClosestLivingPlayer__FG6Point3f(const Point3Words* where, float limit) {
    const Point3Words position = *where;
    int best = -1;
    const int count = nmPlayers;
    for (int i = 0; i < count; ++i) {
        unsigned char* object = *reinterpret_cast<unsigned char**>(player + 23984 * i + 21872);
        if (object == 0) continue;
        if ((*reinterpret_cast<unsigned*>(object + 12) & 0x8) != 0) continue;
        const float* at = reinterpret_cast<const float*>(object + 16);
        const float dx = position.x - at[0];
        const float dy = position.y - at[1];
        const float dz = position.z - at[2];
        const float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (distance < limit) {
            limit = distance;
            best = i;
        }
    }
    return static_cast<signed char>(best);
}
