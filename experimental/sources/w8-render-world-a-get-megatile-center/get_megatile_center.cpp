// Observed getMegatileCenter__FG6Point2 at 0x00265810, 340 bytes. The first
// argument is the address of the result Point2 (two floats); the second is the
// address of the caller's Point2 argument copy, which this function overwrites.
// 1. Both floats of the argument are multiplied by the single-precision constant
//    with bits 0x3ab60b60 (about 1/720), stored at 0x00482060, and written back.
// 2. mt_tiledata (0x00434068) is 32 records of 10 signed bytes. A record whose
//    byte 2 is -1 ends the list. For each record in order, a record matches when
//      float(b7) <= x, float(b8) <= y, x <= float(b7 + b4), y <= float(b8 + b3).
// 3. On the first match the result is x = float(360 * (2 * b7 + b4)) and
//    y = float(360 * (2 * b8 + b3)). With no match, or no records, the result is
//    the scaled argument.
// Inferred: bytes 7 and 8 are a tile origin and bytes 4 and 3 its extents, so the
// result is the tile centre in 1/360 units; 0x3ab60b60 is the nearest-below
// single to 1/720, as the image holds it.
// Unknown: the remaining record bytes and what units the argument uses.
struct Point2 {
    float x;
    float y;
};

extern "C" {
extern signed char mt_tiledata[];

void getMegatileCenter__FG6Point2(Point2* result, Point2* point) {
    const float scale = __builtin_bit_cast(float, 0x3ab60b60u);
    point->x = point->x * scale;
    point->y = point->y * scale;
    const signed char* tile = mt_tiledata;
    for (int index = 0; index < 32 && tile[2] != -1; ++index, tile += 10) {
        if (static_cast<float>(tile[7]) <= point->x && static_cast<float>(tile[8]) <= point->y &&
            point->x <= static_cast<float>(tile[7] + tile[4]) && point->y <= static_cast<float>(tile[8] + tile[3])) {
            result->x = static_cast<float>(360 * (2 * tile[7] + tile[4]));
            result->y = static_cast<float>(360 * (2 * tile[8] + tile[3]));
            return;
        }
    }
    *result = *point;
}
}
