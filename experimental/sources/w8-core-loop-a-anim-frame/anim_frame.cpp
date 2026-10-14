// Observed animFrame__Fv at 0x00163260, 1248 bytes, no calls. When the low four bits of the word drawFrameNm
// are zero, all 300 floats of the 1200-byte table at 0x0034ba08 are overwritten, in a fixed order of stores
// that this source does not preserve. The symbol table names that range groupVolumeScale (a local data
// symbol of exactly 1200 bytes). Every element becomes the float with bits 0x3f7fffff except:
//   index 5, 18, 40 -> bits 0x3f599999;  index 10, 14, 21, 31, 32, 33, 34, 43 -> bits 0x3f666666;
//   index 12 -> bits 0x3f4ccccc;  index 72 -> bits 0x3f266666.
// The five values come from the floating constant pool, read by word. With any other low bits it returns
// at once.
// Inferred: none of the names is claimed to describe the animation system; the body touches no animation
//   state. The table owner is the symbol table's, not this source's.
// Unknown: why a function called animFrame resets a table named for sound group volumes, what the indexes
//   mean, and who else writes the table between resets.
extern "C" {
extern int drawFrameNm;
extern float groupVolumeScale[300];
}

namespace {
float from_bits(unsigned bits) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = bits;
    return value.as_float;
}
}

extern "C" void animFrame__Fv() {
    if (drawFrameNm & 0xf) return;
    const float base = from_bits(0x3f7fffffu);
    const float v65 = from_bits(0x3f266666u);
    const float v90 = from_bits(0x3f666666u);
    const float v85 = from_bits(0x3f599999u);
    const float v80 = from_bits(0x3f4cccccu);
    for (int i = 0; i < 300; ++i) groupVolumeScale[i] = base;
    groupVolumeScale[5] = v85;
    groupVolumeScale[18] = v85;
    groupVolumeScale[40] = v85;
    groupVolumeScale[10] = v90;
    groupVolumeScale[14] = v90;
    groupVolumeScale[21] = v90;
    groupVolumeScale[31] = v90;
    groupVolumeScale[32] = v90;
    groupVolumeScale[33] = v90;
    groupVolumeScale[34] = v90;
    groupVolumeScale[43] = v90;
    groupVolumeScale[12] = v80;
    groupVolumeScale[72] = v65;
}
