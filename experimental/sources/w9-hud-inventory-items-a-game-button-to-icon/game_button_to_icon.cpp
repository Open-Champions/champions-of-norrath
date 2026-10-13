// Observed gameButtonToIcon__Fi at 0x001d05f8, 156 bytes, in the pinned SLUS_205.65.
// Maps a single pad button mask to a small integer, and any other value to -1:
//   0x80 -> 125, 0x20 -> 123, 0x10 -> 124, 0x40 -> 126, 0x8000 -> 127,
//   0x2000 -> 128, 0x1000 -> 129, 0x4000 -> 130, 0x04 -> 131, 0x01 -> 132,
//   0x08 -> 133, 0x02 -> 134.
// Inferred from the one caller that formats it into text: the results are font
// glyph or icon numbers. Which physical button each mask is is not established here.
extern "C" int gameButtonToIcon__Fi(int button) {
    switch (button) {
    case 0x80: return 125;
    case 0x20: return 123;
    case 0x10: return 124;
    case 0x40: return 126;
    case 0x8000: return 127;
    case 0x2000: return 128;
    case 0x1000: return 129;
    case 0x4000: return 130;
    case 0x04: return 131;
    case 0x01: return 132;
    case 0x08: return 133;
    case 0x02: return 134;
    default: return -1;
    }
}
