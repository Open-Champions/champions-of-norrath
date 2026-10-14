// Host check for findAndStripGender__4itemPUs. Not an EE plan input.
#include "find_and_strip_gender.cpp"
signed char language;
extern "C" int ustrlen__FPUs(unsigned short* s) { int n = 0; while (s[n]) n++; return n; }
static int run(int lang, const char* text, unsigned short* out) {
    int n = 0;
    for (; text[n]; n++) out[n] = static_cast<unsigned char>(text[n]);
    out[n] = 0;
    language = static_cast<signed char>(lang);
    return findAndStripGender__4itemPUs(out);
}
int main() {
    unsigned short b[32];
    if (run(4, "ab_m", b) != 1 || b[2] != 0 || b[3] != 'm') return 1;
    if (run(4, "ab_F", b) != 2 || b[2] != 0) return 2;
    if (run(4, "ab_n", b) != 3) return 3;
    if (run(4, "ab_p", b) != 4) return 4;
    if (run(4, "ab_z", b) != 0 || b[2] != 0) return 5;
    if (run(4, "abcm", b) != 0 || b[2] != 'c') return 6;
    if (run(3, "a_ms", b) != 1 || b[1] != 0) return 7;
    if (run(3, "a_FS", b) != 2) return 8;
    if (run(3, "a_mp", b) != 3) return 9;
    if (run(3, "a_FP", b) != 4) return 10;
    if (run(3, "a_mx", b) != 0 || b[1] != 0) return 11;
    if (run(3, "a_sm", b) != 0 || b[1] != 0) return 12;
    if (run(3, "axms", b) != -1 || b[1] != 'x') return 13;
    if (run(2, "a_ms", b) != 0 || b[1] != '_') return 14;
    return 0;
}
