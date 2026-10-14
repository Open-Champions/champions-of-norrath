// Host check for gsm_encode (source: ../w13-startup-fn-a-gsm-encode/gsm_encode.cpp). Not a build-plan input.
//   c++ -std=c++17 -fno-strict-aliasing -o check host_check.cpp && ./check
// The four helper routines are scripted. The check requires: the 160 pre-emphasised words handed to the
// analysis helper equal an independent evaluation of the offset-removal filter from the three state fields
// (and the fields are written back); the helpers receive the sub-frame pointers in order; the 400-byte
// prediction-error area is cleared before use; each sub-frame's error words are added to the history words and
// the 120 newest words are moved to the start of the state; and the 33-byte frame equals an independent
// MSB-first bit writer fed the scripted parameters (magic nibble 13, log-area ratios 6 6 5 5 4 4 3 3 bits, then
// per sub-frame lag 7, gain 2, grid 2, peak 6 and thirteen 3-bit pulses). Return value 33.
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w13-startup-fn-a-gsm-encode/gsm_encode.cpp"

struct Bits {
    std::vector<unsigned char> b;
    int n = 0;
    void put(unsigned v, int w) {
        for (int i = w - 1; i >= 0; --i) {
            if (n % 8 == 0) b.push_back(0);
            if ((v >> i) & 1) b.back() |= 0x80 >> (n % 8);
            ++n;
        }
    }
};

static word script_lar[8], script_nc[4], script_bc[4], script_mc[4], script_xmax[4], script_xm[52];
static word seen_so[160], seen_lar_at_filter[8];
static word seen_e_before[4][50];
static int ltp_calls, rpe_calls, lpc_calls, staf_calls;
static word* so_base;
static word* dp_first;
static bool order_ok = true;
static bool e_error_clear = true;

extern "C" void Gsm_LPC_Analysis(word* so, word* LARc) {
    ++lpc_calls; so_base = so;
    for (int i = 0; i < 160; ++i) seen_so[i] = so[i];
    for (int i = 0; i < 8; ++i) LARc[i] = script_lar[i];
}
extern "C" void Gsm_Short_Term_Analysis_Filter(void* state, word* LARc, word* so) {
    ++staf_calls;
    if (so != so_base) order_ok = false;
    for (int i = 0; i < 8; ++i) seen_lar_at_filter[i] = LARc[i];
}
extern "C" void Gsm_Long_Term_Predictor(void* state, word* d, word* dp, word* e, word* dpp, word* Nc, word* bc) {
    int j = ltp_calls++;
    if (d != so_base + 40 * j || dp != dpp) order_ok = false;
    if (j == 0) dp_first = dp;
    else if (dp != dp_first + 40 * j) order_ok = false;
    for (int i = -5; i < 45; ++i) if (e[i] != 0 && j == 0) e_error_clear = false;
    for (int i = 0; i < 40; ++i) e[i] = (j + 1) * 100 + i;
    *Nc = script_nc[j]; *bc = script_bc[j];
}
extern "C" void Gsm_RPE_Encoding(word* e, word* xmaxc, word* Mc, word* xMc) {
    int j = rpe_calls++;
    for (int i = 0; i < 40; ++i) if (e[i] != (j + 1) * 100 + i) order_ok = false;
    *xmaxc = script_xmax[j]; *Mc = script_mc[j];
    for (int i = 0; i < 13; ++i) xMc[i] = script_xm[13 * j + i];
}

int main() {
    int bad = 0;
    static const int lar_w[8] = {6, 6, 5, 5, 4, 4, 3, 3};
    for (int round = 0; round < 6; ++round) {
        unsigned seed = 777u + round * 104729u;
        auto rnd = [&](int w) { seed = seed * 1103515245u + 12345u; return (seed >> 16) & ((1u << w) - 1); };
        alignas(16) static unsigned char state[4096];
        for (int i = 0; i < 4096; ++i) state[i] = (unsigned char)rnd(8);
        word z1 = (round % 2) ? 0 : (word)rnd(12) - 2000;
        word l2 = (round == 0) ? 0 : (word)rnd(16) * 40000 - 1000000;
        int mp0 = (round == 2) ? 0 : (int)rnd(14) - 8000;
        *reinterpret_cast<word*>(state + 2240) = z1;
        *reinterpret_cast<word*>(state + 2248) = l2;
        *reinterpret_cast<int*>(state + 2256) = mp0;
        short samples[160];
        for (int k = 0; k < 160; ++k) samples[k] = round == 3 ? (short)((k & 1) ? 32767 : -32768) : (short)(rnd(16) - 32768);
        for (int i = 0; i < 8; ++i) script_lar[i] = rnd(lar_w[i]);
        for (int j = 0; j < 4; ++j) {
            script_nc[j] = rnd(7); script_bc[j] = rnd(2); script_mc[j] = rnd(2); script_xmax[j] = rnd(6);
            for (int i = 0; i < 13; ++i) script_xm[13 * j + i] = rnd(3);
        }
        // Independent expectation of the pre-emphasis words and of the state history.
        word want_so[160];
        word ez1 = z1, el2 = l2, emp = mp0;
        for (int k = 0; k < 160; ++k) {
            word s = (samples[k] >> 3) * 4;
            el2 += (s - ez1) * 32768;
            ez1 = s;
            word fb = (emp * -28672) >> 15;
            emp = (el2 + 16384) >> 15;
            want_so[k] = emp + fb;
        }
        word hist[280];
        std::memcpy(hist, state, sizeof hist);
        for (int j = 0; j < 4; ++j) for (int i = 0; i < 40; ++i) hist[120 + 40 * j + i] += (j + 1) * 100 + i;
        word want_hist[120];
        for (int i = 0; i < 120; ++i) want_hist[i] = hist[160 + i];
        ltp_calls = rpe_calls = lpc_calls = staf_calls = 0; order_ok = true; e_error_clear = true;
        unsigned char frame[40]; std::memset(frame, 0xee, sizeof frame);
        int r = gsm_encode(state, samples, frame);
        if (r != 33) { std::printf("round %d return %d\n", round, r); ++bad; }
        if (lpc_calls != 1 || staf_calls != 1 || ltp_calls != 4 || rpe_calls != 4) { std::puts("call counts"); return 1; }
        if (!order_ok) { std::printf("round %d helper argument order\n", round); ++bad; }
        if (!e_error_clear) { std::printf("round %d error area not cleared\n", round); ++bad; }
        for (int k = 0; k < 160; ++k) if (seen_so[k] != want_so[k]) { std::printf("round %d so[%d]\n", round, k); ++bad; break; }
        for (int i = 0; i < 8; ++i) if (seen_lar_at_filter[i] != script_lar[i]) { std::printf("round %d lar %d\n", round, i); ++bad; }
        if (*reinterpret_cast<word*>(state + 2240) != ez1 || *reinterpret_cast<word*>(state + 2248) != el2 ||
            *reinterpret_cast<int*>(state + 2256) != (int)emp) { std::printf("round %d state fields\n", round); ++bad; }
        if (std::memcmp(state, want_hist, sizeof want_hist)) { std::printf("round %d history move\n", round); ++bad; }
        Bits bits; bits.put(13, 4);
        for (int i = 0; i < 8; ++i) bits.put((unsigned)script_lar[i], lar_w[i]);
        for (int j = 0; j < 4; ++j) {
            bits.put((unsigned)script_nc[j], 7); bits.put((unsigned)script_bc[j], 2);
            bits.put((unsigned)script_mc[j], 2); bits.put((unsigned)script_xmax[j], 6);
            for (int i = 0; i < 13; ++i) bits.put((unsigned)script_xm[13 * j + i], 3);
        }
        if (bits.b.size() != 33 || std::memcmp(bits.b.data(), frame, 33)) { std::printf("round %d frame bytes\n", round); ++bad; }
        if (frame[33] != 0xee) { std::puts("wrote past the frame"); ++bad; }
    }
    return bad;
}
