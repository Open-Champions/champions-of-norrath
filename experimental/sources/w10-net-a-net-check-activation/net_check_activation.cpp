// Observed netCheckActivation__FG6Point3f at 0x00135658, 172 bytes. The Point3
// argument is passed by address to a caller-made copy, and the range arrives in
// the first float register. The routine copies the point, then visits the first
// nmPlayers records of the player array (23984-byte stride). For each record
// whose word at offset 21872 is a non-null object pointer it takes the point at
// offset 16 of that object, sums the squared coordinate differences in x, y, z
// order and sets the result to 1 when the sum is below range * range. The loop
// does not stop early. The result is 0 or 1; a count of zero or less returns 0.
// Inferred: this is an "is any player close enough" test used for activation in
// network games; the word at 21872 is the record's player object.
// Unknown: the name of that word, and the behaviour for NaN inputs beyond what
// the single-precision compare gives.
struct Point3 {
    float x;
    float y;
    float z;
};

extern "C" {
extern int nmPlayers;
extern unsigned char player[];
}

#ifdef NORRATH_W10NA_HOST
extern "C" const unsigned char* norrath_w10na_word_to_pointer(unsigned word);
#define NORRATH_W10NA_POINTER(word) norrath_w10na_word_to_pointer(word)
#else
#define NORRATH_W10NA_POINTER(word) reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" int netCheckActivation__FG6Point3f(const Point3* position, float range) {
    Point3 point = *position;
    int result = 0;
    for (int i = 0; i < nmPlayers; ++i) {
        unsigned word = *reinterpret_cast<const unsigned*>(player + i * 23984 + 21872);
        if (word == 0) continue;
        const Point3* other = reinterpret_cast<const Point3*>(NORRATH_W10NA_POINTER(word) + 16);
        float dx = point.x - other->x;
        float dy = point.y - other->y;
        float dz = point.z - other->z;
        float dist = dx * dx + dy * dy + dz * dz;
        if (dist < range * range) result = 1;
    }
    return result;
}
