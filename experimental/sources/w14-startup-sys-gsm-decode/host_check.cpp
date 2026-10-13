// Host check for gsm_decode (source: ../w13-startup-fn-a-gsm-decode/gsm_decode.cpp). Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// A frame is packed with a generic MSB-first bit writer from chosen parameters (magic nibble 13, eight
// log-area ratios of 6 6 5 5 4 4 3 3 bits, then per sub-frame lag 7, gain 2, grid 2, peak 6 and thirteen
// 3-bit pulses). The three helper routines are scripted to record their arguments. The check requires that
// every recorded parameter equals the packed value, that the 160-word array handed to the short-term
// filter is the four long-term output blocks in order, that the de-emphasis (x*28672>>15, add, double with
// saturation, clear the low three bits) matches an independent computation over saturation-prone and
// ordinary samples, that the state word at +2552 is updated, and that a bad magic nibble returns 0 and touches
// neither the state nor the output.
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w13-startup-fn-a-gsm-decode/gsm_decode.cpp"

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

struct Rpe { word xmaxc, Mc, xmc[13]; };
struct Lts { word Nc, bc; };
static std::vector<Rpe> rpe_calls;
static std::vector<Lts> lts_calls;
static word sts_lar[8], sts_wt[160];
static void* sts_state;
static int sts_calls;
static short sample_script[160];
static word* g_drp;

extern "C" void Gsm_RPE_Decoding(word xmaxc, word Mc, word* xmc, word* erp) {
    Rpe r{xmaxc, Mc, {}};
    for (int i = 0; i < 13; ++i) r.xmc[i] = xmc[i];
    rpe_calls.push_back(r);
    for (int k = 0; k < 40; ++k) erp[k] = 1000 * (int)rpe_calls.size() + k;
}
extern "C" void Gsm_Long_Term_Synthesis_Filtering(void* state, word Nc, word bc, word* erp, word* drp) {
    lts_calls.push_back({Nc, bc});
    g_drp = drp;
    for (int k = 0; k < 40; ++k) drp[k] = erp[k] * 3 + (int)lts_calls.size();
}
extern "C" void Gsm_Short_Term_Synthesis_Filter(void* state, word* LARc, word* wt, short* target) {
    ++sts_calls; sts_state = state;
    std::memcpy(sts_lar, LARc, sizeof sts_lar);
    std::memcpy(sts_wt, wt, sizeof sts_wt);
    std::memcpy(target, sample_script, sizeof sample_script);
}

static long long deemph(long long msr, const short* in, short* out) {
    for (int k = 0; k < 160; ++k) {
        msr = in[k] + ((msr * 28672) >> 15);
        long long d = msr + msr;
        if (d > 32767) d = 32767;
        if (d < -32768) d = -32768;
        out[k] = (short)(d & 0xfff8);
    }
    return msr;
}

int main() {
    int bad = 0;
    alignas(16) static unsigned char state[4096];
    static const int lar_w[8] = {6, 6, 5, 5, 4, 4, 3, 3};
    for (int round = 0; round < 6; ++round) {
        unsigned seed = 12345u + round * 7919u;
        auto rnd = [&](int w) { seed = seed * 1103515245u + 12345u; return (seed >> 16) & ((1u << w) - 1); };
        Bits bits; bits.put(13, 4);
        word lar[8], nc[4], bcv[4], mc[4], xmax[4], xm[52];
        for (int i = 0; i < 8; ++i) { lar[i] = rnd(lar_w[i]); bits.put((unsigned)lar[i], lar_w[i]); }
        for (int j = 0; j < 4; ++j) {
            nc[j] = rnd(7); bcv[j] = rnd(2); mc[j] = rnd(2); xmax[j] = rnd(6);
            bits.put((unsigned)nc[j], 7); bits.put((unsigned)bcv[j], 2); bits.put((unsigned)mc[j], 2); bits.put((unsigned)xmax[j], 6);
            for (int i = 0; i < 13; ++i) { xm[13 * j + i] = rnd(3); bits.put((unsigned)xm[13 * j + i], 3); }
        }
        if (bits.b.size() != 33) { std::puts("frame size"); return 1; }
        // Sample scripts: extremes on some rounds, ordinary values on others.
        for (int k = 0; k < 160; ++k) {
            if (round % 3 == 0) sample_script[k] = (short)((k % 2) ? 32767 : -32768);
            else if (round % 3 == 1) sample_script[k] = (short)(k * 97 - 7000);
            else sample_script[k] = (short)(((int)rnd(16)) - 32768);
        }
        std::memset(state, 0x5a, sizeof state);
        long long msr0 = (round == 0) ? 0 : (round == 1 ? 0x7fffffffLL : (round == 2 ? -0x7fffffffLL : 123456789LL * (round - 2)));
        std::memcpy(state + 2552, &msr0, 8);
        rpe_calls.clear(); lts_calls.clear(); sts_calls = 0;
        short out[160];
        int r = gsm_decode(state, bits.b.data(), out);
        if (r != 160) { std::printf("round %d return %d\n", round, r); ++bad; }
        if (rpe_calls.size() != 4 || lts_calls.size() != 4 || sts_calls != 1) { std::puts("call counts"); return 1; }
        for (int j = 0; j < 4; ++j) {
            if (rpe_calls[j].xmaxc != xmax[j] || rpe_calls[j].Mc != mc[j]) { std::printf("round %d rpe %d head\n", round, j); ++bad; }
            for (int i = 0; i < 13; ++i) if (rpe_calls[j].xmc[i] != xm[13 * j + i]) { std::printf("round %d pulse %d.%d\n", round, j, i); ++bad; }
            if (lts_calls[j].Nc != nc[j] || lts_calls[j].bc != bcv[j]) { std::printf("round %d lts %d\n", round, j); ++bad; }
        }
        for (int i = 0; i < 8; ++i) if (sts_lar[i] != lar[i]) { std::printf("round %d lar %d\n", round, i); ++bad; }
        if (sts_state != state || g_drp != (word*)(state + 960)) { std::puts("state pointers"); ++bad; }
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 40; ++k)
                if (sts_wt[40 * j + k] != ((word)(1000 * (j + 1) + k)) * 3 + (j + 1)) { std::printf("round %d wt %d.%d\n", round, j, k); ++bad; }
        short want[160];
        long long msr_end = deemph(msr0, sample_script, want);
        if (std::memcmp(want, out, sizeof want)) { std::printf("round %d samples\n", round); ++bad; }
        long long msr_got; std::memcpy(&msr_got, state + 2552, 8);
        if (msr_got != msr_end) { std::printf("round %d msr %lld vs %lld\n", round, msr_got, msr_end); ++bad; }
    }
    // Bad magic: return 0, no helper call, state untouched, output untouched.
    for (int m = 0; m < 16; ++m) {
        if (m == 13) continue;
        unsigned char frame[33] = {};
        frame[0] = (unsigned char)((m << 4) | 0x0f);
        std::memset(state, 0x33, sizeof state);
        short out[160]; std::memset(out, 0x77, sizeof out);
        rpe_calls.clear(); sts_calls = 0;
        if (gsm_decode(state, frame, out) != 0 || !rpe_calls.empty() || sts_calls) { std::printf("magic %d\n", m); ++bad; }
        for (unsigned char c : state) if (c != 0x33) { std::puts("state touched"); ++bad; break; }
        for (short s : out) if (s != 0x7777) { std::puts("output touched"); ++bad; break; }
    }
    return bad;
}
