// Observed makeCollisionList__FP12_worldHeaderffffPP13collisionParti at 0x00175d88, 596 bytes.
// Arguments: the world header, four floats (a, b, c, d), an output array of part pointers and
// the array's capacity. It returns nothing the callers use. Callers (worldClipMotion and
// siblings) pass a, b as the low corner and c, d as the high corner of an x/y box.
// Steps:
//  1. a, b, c, d are each multiplied by the float 0x3bcccccc (the word at 0x0048001c) and
//     floored with floorf, then converted to int: ia, ib, ic, id.
//  2. Using the header's words +8 and +12 (origin for the first and second index), +16 and
//     +20 (extent of each), the box is clamped to the grid: columns from max(ia - word8, 0)
//     to min(ic - word8, word16 - 1) and rows from max(ib - word12, 0) to
//     min(id - word12, word20 - 1).
//  3. For each row, then each column in range, the cell is the word at table[row * word16 +
//     column], table being the pointer in header word +24. A cell points to a list of
//     halfword part numbers ended by -1 (an empty cell has -1 first). A part is the 36-byte
//     record at (header word +32) + 36 * number.
//  4. A part is skipped when its word at +8 equals collisionFrame (already listed this
//     query), when bit 2 of its halfword at +16 is set, or when the box misses its bounds:
//     c < (float)s16[0], d < (float)s16[1], (float)s16[2] < a, or (float)s16[3] < b.
//  5. Otherwise collisionFrame is stored in the part's word +8 and the part pointer is
//     appended to the output. When the count reaches capacity - 1 right after an append
//     the whole scan stops. The output is always ended with a zero entry at the count.
// Inferred: the part bounds are x/y minimum then maximum halfwords because they are tested
// that way against the box; the scan order (rows outer, columns inner) is from the loop
// shape. collisionFrame is advanced by the callers, not here.
// Unknown: names for the header and part fields; behavior for capacity below 2, where the
// image appends one entry before testing the count, as this version does.
extern "C" float floorf(float value);
extern "C" int collisionFrame;

struct CollisionPart {
    short bounds[4];
    int frame_stamp;
    unsigned char gap[4];
    unsigned short flags;
    unsigned char rest[18];
};
struct CollisionWorld {
    unsigned char before_origin[8];
    int origin_column;
    int origin_row;
    int columns;
    int rows;
    short** cells;
    unsigned char gap[4];
    CollisionPart* parts;
};
#if defined(__mips__)
static_assert(sizeof(CollisionPart) == 36 && __builtin_offsetof(CollisionPart, frame_stamp) == 8 &&
                  __builtin_offsetof(CollisionPart, flags) == 16, "observed part layout");
static_assert(__builtin_offsetof(CollisionWorld, origin_column) == 8 && __builtin_offsetof(CollisionWorld, origin_row) == 12 &&
                  __builtin_offsetof(CollisionWorld, columns) == 16 && __builtin_offsetof(CollisionWorld, rows) == 20 &&
                  __builtin_offsetof(CollisionWorld, cells) == 24 && __builtin_offsetof(CollisionWorld, parts) == 32,
              "observed header layout");
#endif

extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(CollisionWorld* world, float a, float b, float c, float d,
                                                                         CollisionPart** list, int capacity) {
    const float scale = __builtin_bit_cast(float, 0x3bccccccu);
    const int low_column = static_cast<int>(floorf(a * scale));
    const int low_row = static_cast<int>(floorf(b * scale));
    const int high_column = static_cast<int>(floorf(c * scale));
    const int high_row = static_cast<int>(floorf(d * scale));
    int row_first = low_row - world->origin_row;
    if (row_first < 0) row_first = 0;
    int row_last = high_row - world->origin_row;
    if (world->rows - 1 < row_last) row_last = world->rows - 1;
    int column_last = high_column - world->origin_column;
    if (world->columns - 1 < column_last) column_last = world->columns - 1;
    int column_first = low_column - world->origin_column;
    if (column_first < 0) column_first = 0;
    int count = 0;
    for (int row = row_first; row <= row_last; ++row) {
        for (int column = column_first; column <= column_last; ++column) {
            const short* numbers = world->cells[row * world->columns + column];
            while (*numbers != -1) {
                CollisionPart* part = &world->parts[*numbers];
                ++numbers;
                const int stamp = collisionFrame;
                if (part->frame_stamp != stamp && (part->flags & 2u) == 0u && !(c < static_cast<float>(part->bounds[0])) &&
                    !(d < static_cast<float>(part->bounds[1])) && !(static_cast<float>(part->bounds[2]) < a) &&
                    !(static_cast<float>(part->bounds[3]) < b)) {
                    list[count++] = part;
                    part->frame_stamp = stamp;
                    if (count == capacity - 1) goto done;
                }
            }
        }
    }
done:
    list[count] = nullptr;
}
