// Host check only; not an EE build input. Build the source with -DGsm_RPE_Decoding=w15_rpe_decoding; gsm_FAC is
// defined here with the observed eight values. The reference below restates the rules of the comment in a different
// shape (indexed output rather than the fall-through interleave) and is compared over many inputs.
#include <cstdio>
typedef long long word;
extern "C" {
word gsm_FAC[8] = {18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767};
void w15_rpe_decoding(word xmaxc, word grid, const word* pulses, word* out);
}
static void reference(word xmaxc, word grid, const word* pulses, word* out, int& written) {
    word exp = xmaxc > 15 ? (xmaxc >> 3) - 1 : 0;
    word mant = xmaxc - exp * 8;
    if (mant == 0) { exp = -4; mant = 7; }
    else { while (mant < 8) { mant = mant * 2 + 1; --exp; } mant -= 8; }
    word shift = 6 - exp;
    if (shift > 32767) shift = 32767;
    if (shift < -32768) shift = -32768;
    word before = shift - 1;
    word round_word;
    if (before >= 32767) round_word = static_cast<int>(0x80000000u);
    else { if (before < -32768) before = -32768; round_word = static_cast<int>(1u << (before & 31)); }
    word values[13];
    for (int i = 0; i < 13; ++i) {
        word t = pulses[i] * 8192 - 28672;
        t = gsm_FAC[mant] * t / 32768 - ((gsm_FAC[mant] * t) % 32768 < 0 ? 1 : 0);   // floor division
        word sum = t + round_word;
        if (sum > 32767) sum = 32767;
        if (sum < -32768) sum = -32768;
        values[i] = sum >> (shift & 63);
    }
    written = 0;
    if (grid >= 0 && grid <= 3) {
        for (int i = 0; i < 13; ++i) {
            for (int z = 0; z < (i == 0 ? grid : 2); ++z) out[written++] = 0;
            out[written++] = values[i];
        }
        for (word g = grid + 1; g < 4; ++g) out[written++] = 0;
    } else {
        for (word g = grid + 1; g < 4; ++g) out[written++] = 0;
    }
}
int main() {
    unsigned long long s = 88172645463325252ull;
    for (int round = 0; round < 40000; ++round) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        word pulses[13];
        for (int i = 0; i < 13; ++i) {
            s ^= s << 13; s ^= s >> 7; s ^= s << 17;
            pulses[i] = (round % 3 == 0) ? static_cast<word>(s % 8) : static_cast<word>(s % 200) - 100;
        }
        const word xmaxc = static_cast<word>(s % 70);
        const word grid = static_cast<word>((s >> 8) % 4) + (round % 9 == 0 ? -3 : 0) + (round % 11 == 0 ? 2 : 0);
        word got[80], want[80];
        for (int i = 0; i < 80; ++i) got[i] = want[i] = 0x1234;
        int written;
        reference(xmaxc, grid, pulses, want, written);
        w15_rpe_decoding(xmaxc, grid, pulses, got);
        for (int i = 0; i < 80; ++i) if (got[i] != want[i]) { std::printf("mismatch round %d index %d\n", round, i); return 1; }
    }
    std::puts("rpe_decoding ok");
    return 0;
}
