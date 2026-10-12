// Observed getMegatileOriginalCenter__FG6Point2 at 0x00265968, 348 bytes. Same
// calling shape and search as getMegatileCenter__FG6Point2 (0x00265810): the
// first argument is the address of the result Point2, the second the address of
// the caller's Point2 copy, which is overwritten.
// 1. Both floats of the argument are multiplied by the single-precision constant
//    with bits 0x3ab60b60 (about 1/720), stored at 0x00482064, and written back.
// 2. The same 32-record list at mt_tiledata (0x00434068, 10 signed bytes each,
//    byte 2 equal to -1 ends it) is searched with the same test:
//      float(b7) <= x, float(b8) <= y, x <= float(b7 + b4), y <= float(b8 + b3).
// 3. On the first match the result is x = float(360 * (2 * b0 + b6)) and
//    y = float(360 * (2 * b1 + b5)), from other bytes of the record than the
//    other function uses. With no match the result is the scaled argument.
// Inferred: this reads the record's other rectangle (bytes 0, 1, 5, 6), which the
// name suggests is the original placement; the image only shows the byte
// choices. The constant is its own copy in the image, with the same bits.
// Unknown: the byte meanings and the units of the argument.
struct Point2 {
    float x;
    float y;
};

extern "C" {
extern signed char mt_tiledata[];

void getMegatileOriginalCenter__FG6Point2(Point2* result, Point2* point) {
    const float scale = __builtin_bit_cast(float, 0x3ab60b60u);
    point->x = point->x * scale;
    point->y = point->y * scale;
    const signed char* tile = mt_tiledata;
    for (int index = 0; index < 32 && tile[2] != -1; ++index, tile += 10) {
        if (static_cast<float>(tile[7]) <= point->x && static_cast<float>(tile[8]) <= point->y &&
            point->x <= static_cast<float>(tile[7] + tile[4]) && point->y <= static_cast<float>(tile[8] + tile[3])) {
            result->x = static_cast<float>(360 * (2 * tile[0] + tile[6]));
            result->y = static_cast<float>(360 * (2 * tile[1] + tile[5]));
            return;
        }
    }
    *result = *point;
}
}
