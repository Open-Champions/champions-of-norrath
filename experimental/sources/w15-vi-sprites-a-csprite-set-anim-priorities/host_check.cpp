// Host check only; not a build input.
#include "set_anim_priorities.cpp"
#include <cstring>
int main() {
    alignas(8) static unsigned char s[4096], e[4096];
    std::memset(s, 0x5a, sizeof s); std::memset(e, 0x5a, sizeof e);
    if (SetAnimPriorities__9VICSprite(s) != 0) return 1;
    auto put = [&](int v, const int* offs, int n) { for (int i = 0; i < n; ++i) std::memcpy(e + offs[i], &v, 4); };
    { static const int o[] = {1220, 1224}; put(0, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {1272, 1276, 1328}; put(1, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {440, 444, 492, 544, 596, 648, 700, 752, 960, 1012, 1064, 1116, 1324}; put(2, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {804, 856, 860, 908, 912}; put(3, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {2208, 2212, 3352, 3356, 3404, 3408, 3508, 3512, 3560, 3564, 3612, 3616, 3716, 3720, 3768, 3772, 3820, 3824}; put(4, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {1584, 1588, 1636, 1640, 1688, 1692, 1740, 1744, 1792, 1796, 1844, 1848, 1896, 1900, 1948, 1952, 2000, 2004, 2052, 2056, 2104, 2108, 2156, 2160, 2260, 2264, 2312, 2316, 2364, 2368, 2416, 2420, 2468, 2472, 2520, 2524, 2572, 2576, 2624, 2628, 2676, 2728, 2780, 2784, 2832, 2836, 3040, 3044, 3092, 3096, 3144, 3196}; put(5, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {496, 548, 600, 652, 704, 756, 964, 1016, 1068, 1120, 1376, 3148, 3200}; put(6, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {808, 1168, 1380, 2680, 2732}; put(7, o, sizeof o / sizeof o[0]); }
    { static const int o[] = {1172}; put(8, o, sizeof o / sizeof o[0]); }
    return std::memcmp(s, e, sizeof s) ? 2 : 0;
}
