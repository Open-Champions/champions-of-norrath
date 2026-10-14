// Observed: SetNodeFixScale__9VIHSpriteiRC7VIVect3 at 0x010636a8 is 48 bytes. With (this, index, v) it takes the pointer at this+72,
// adds index * 256 + 112 and copies the three floats of v there (offsets 0, 4, 8 of that address). It returns zero.
// Inferred: the pointer at +72 is an array of 256-byte node records and +112 within a record is a three-float scale.
// Unknown: the return value's purpose (it is zero on this path, the only path) and the name of the record type.
extern "C" {
int SetNodeFixScale__9VIHSpriteiRC7VIVect3(char* self, int index, const float* v) {
    float* dst = (float*)(*(char**)(self + 72) + index * 256 + 112);
    dst[0] = v[0];
    dst[1] = v[1];
    dst[2] = v[2];
    return 0;
}
}
