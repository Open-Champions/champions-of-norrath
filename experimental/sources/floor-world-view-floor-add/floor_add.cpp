// Observed floorAdd__FG6Point3sfii at 0x00191ef8, 956 bytes.
// The point is the 12 bytes addressed by the first argument. The second
// argument is shifted left 16 and arithmetically back, which is the signed
// half norrath_floor_add reads. The float argument is the extent. The next
// integer is the kind: blood level zero and kind zero return, and kinds 1
// and 2 return. The last integer is the delayed-record flag. The published
// body is norrath_floor_add. This unit is the name the partial build imports.
extern "C" void norrath_floor_add(const unsigned char* point, int type_word, float extent,
                                  int kind, int flags);

extern "C" void floorAdd__FG6Point3sfii(const unsigned char* point, int type_word, float extent,
                                        int kind, int flags) {
    norrath_floor_add(point, type_word, extent, kind, flags);
}
