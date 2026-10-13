// Observed getNextRoutePoint__8CreatureP6Point3G6Point3ffi at 0x00113270, 216 bytes.
// Arguments: creature, output Point3 pointer, a Point3 (passed by address), two floats, one int.
// Entries are int pairs at object+580+8*j, j = 0..15, with x == -1 ending the route; the current
// index i is the signed int at object+708. The Point3 argument is copied to a local first.
//  - A null output pointer returns 0 and does nothing else.
//  - Otherwise out->x and out->y are set to -1.0 (out->z is untouched) and the result starts at 0.
//  - If i >= 16 the result 0 is returned.
//  - If entry i has x == -1, plotRoute__8CreatureG6Point3ffi is called with the creature, the
//    local Point3, the two floats and the int, and 0 is returned (the index is not changed).
//  - Otherwise the index becomes i + 1 (stored even when that is 16). If i + 1 is 16 or that entry
//    has x == -1, plotRoute is called as above and 0 is returned. Else out->x and out->y become the
//    next entry's two ints converted to float, and 1 is returned.
// The plotRoute result is ignored. Inferred: this advances a route and falls back to planning one
// when it runs out. Unknown: what the Point3, the floats and the int mean (they are only forwarded)
// and the field names. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" void plotRoute__8CreatureG6Point3ffi(void* self, const Point3* point, float a, float b, int c);

extern "C" int getNextRoutePoint__8CreatureP6Point3G6Point3ffi(void* self, Point3* out, const Point3* from, float a, float b, int c) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const Point3 local = *from;
    if (out == nullptr) return 0;
    out->x = -1.0f;
    out->y = -1.0f;
    int* indexSlot = reinterpret_cast<int*>(object + 708);
    const int index = *indexSlot;
    if (index >= 16) return 0;
    const int* table = reinterpret_cast<const int*>(object + 580);
    if (table[2 * index] != -1) {
        const int next = index + 1;
        *indexSlot = next;
        if (next != 16 && table[2 * next] != -1) {
            out->x = static_cast<float>(table[2 * next]);
            out->y = static_cast<float>(table[2 * next + 1]);
            return 1;
        }
    }
    plotRoute__8CreatureG6Point3ffi(self, &local, a, b, c);
    return 0;
}
