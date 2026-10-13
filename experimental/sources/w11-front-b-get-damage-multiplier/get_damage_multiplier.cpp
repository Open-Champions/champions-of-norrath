// Observed getDamageMultiplier__4itemPQ24item11description at 0x00108230, 16
// bytes. It returns the single-precision constant 1.0 and does not read its
// argument (the description pointer). Its one caller family is the item damage
// range routines, which multiply by the result.
// Unknown: whether a different multiplier was ever meant to be applied here.
extern "C" float getDamageMultiplier__4itemPQ24item11description(void*) {
    return 1.0f;
}
