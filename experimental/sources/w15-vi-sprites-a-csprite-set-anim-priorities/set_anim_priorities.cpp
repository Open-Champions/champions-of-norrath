// Observed: SetAnimPriorities__9VICSprite at 0x010b20d0, 488 bytes. It writes 112 words of the object and returns zero; it reads nothing. All
// offsets are from this and every write is a 32-bit word. The values are: 0 at +1220 and +1224; 1 at +1272, +1276 and +1328; 2 at +440, +444, +492,
// +544, +596, +648, +700, +752, +960, +1012, +1064, +1116 and +1324; 3 at +804, +856, +860, +908 and +912; 4 at eighteen words in +2208..+3824;
// 5 at fifty-two words in +1584..+3196; 6 at thirteen words from +496 to +3200; 7 at +808, +1168, +1380, +2680 and +2732; 8 at +1172 (the
// complete lists are the arrays below). Most of the offsets are spaced by 52 bytes, and many come in pairs four bytes apart.
// Inferred: the object holds a table of 52-byte per-animation records whose first two words here are small ranks (priority values 0..8).
// Unknown: what the records are, why only the listed records get a rank, and what the rank orders; the rank names are not known.
extern "C" {
int SetAnimPriorities__9VICSprite(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    static const unsigned short kValue0[] = {1220, 1224};
    static const unsigned short kValue1[] = {1272, 1276, 1328};
    static const unsigned short kValue2[] = {440, 444, 492, 544, 596, 648, 700, 752, 960, 1012, 1064, 1116, 1324};
    static const unsigned short kValue3[] = {804, 856, 860, 908, 912};
    static const unsigned short kValue4[] = {2208, 2212, 3352, 3356, 3404, 3408, 3508, 3512, 3560, 3564, 3612, 3616, 3716, 3720, 3768, 3772, 3820,
        3824};
    static const unsigned short kValue5[] = {1584, 1588, 1636, 1640, 1688, 1692, 1740, 1744, 1792, 1796, 1844, 1848, 1896, 1900, 1948, 1952, 2000,
        2004, 2052, 2056, 2104, 2108, 2156, 2160, 2260, 2264, 2312, 2316, 2364, 2368, 2416, 2420, 2468, 2472, 2520, 2524, 2572, 2576, 2624, 2628,
        2676, 2728, 2780, 2784, 2832, 2836, 3040, 3044, 3092, 3096, 3144, 3196};
    static const unsigned short kValue6[] = {496, 548, 600, 652, 704, 756, 964, 1016, 1068, 1120, 1376, 3148, 3200};
    static const unsigned short kValue7[] = {808, 1168, 1380, 2680, 2732};
    static const unsigned short kValue8[] = {1172};
    static const unsigned short* const kLists[9] = {kValue0, kValue1, kValue2, kValue3, kValue4, kValue5, kValue6, kValue7, kValue8};
    static const unsigned char kCounts[9] = {2, 3, 13, 5, 18, 52, 13, 5, 1};
    for (int v = 0; v < 9; ++v)
        for (int i = 0; i < kCounts[v]; ++i) *reinterpret_cast<int*>(bytes + kLists[v][i]) = v;
    return 0;
}
}
