// Observed P_FaceSprite__FG6Point3ffiiiiP8_texturesi at 0x001815c8, 148 bytes.
// Arguments: a0 = address of the by-value Point3 (3 singles), F12 and F13 = two
// singles, a1/a2/a3/t0 = four ints, t1 = _texture pointer, t2 = a short (the original
// sign-extends the low halfword), t3 = an int whose low halfword is stored.
//  1. nmSingle (0x004869f8) equal to 1024 returns with nothing written.
//  2. Otherwise the record at singleBuffer (0x006ec880) + nmSingle * 32 is filled and
//     nmSingle is incremented. The record is 32 bytes (the index is shifted by 5).
//     Offsets: 0..11 the point; 12 F13; 16 F12; 20, 21, 22, 23 the low bytes of the
//     four ints; 24 the texture pointer; 28 the halfword from t2; 30 the halfword from t3.
//  3. The point is copied through a stack temporary; the record never aliases the
//     argument in a legitimate call, and that temporary has no other reader.
// Unknown: the meaning of the four byte fields and the two floats (inferred only:
// colour/size style values); whether the record layout is shared with other writers.
// Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Texture;

struct SingleRecord {
    Point3 position;
    float second_float;
    float first_float;
    unsigned char byte_20, byte_21, byte_22, byte_23;
    Texture* texture;
    short half_28;
    short half_30;
};

extern "C" {
extern int nmSingle;
extern unsigned char singleBuffer[];

void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const Point3* position, float first_float, float second_float,
                                              int a, int b, int c, int d, Texture* texture, short half_28,
                                              int half_30) {
    const Point3 copy = *position;
    if (nmSingle == 1024) return;
    const int index = nmSingle;
    nmSingle = index + 1;
    SingleRecord* record = reinterpret_cast<SingleRecord*>(singleBuffer) + index;
    record->position = copy;
    record->half_28 = half_28;
    record->first_float = first_float;
    record->second_float = second_float;
    record->byte_20 = static_cast<unsigned char>(a);
    record->byte_21 = static_cast<unsigned char>(b);
    record->byte_22 = static_cast<unsigned char>(c);
    record->byte_23 = static_cast<unsigned char>(d);
    record->texture = texture;
    record->half_30 = static_cast<short>(half_30);
}
}
